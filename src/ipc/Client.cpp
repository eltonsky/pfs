//TODO: when exception, stop a call or a connection, instead of std::abort. it's a bit hash.

#include "Client.h"

const long Client::_call_wait_time = 100;
bool Client::_should_stop = false;

/// Client::Connection

Client::Connection::Connection(shared_ptr<tcp::endpoint> ep) :
                                _ep(ep) ,
                                _recv_started(false),
                                _should_close(false){}

Client::Connection::Connection(shared_ptr<tcp::endpoint> ep, int index_) :
                                _ep(ep) ,
                                _recv_started(false),
                                _should_close(false),
                                index{index_} {}

bool Client::Connection::connect(shared_ptr<tcp::endpoint> ep,
                                 shared_ptr<Call> call) {
    try{
        // if conn is not closed, just use it.
        if(_should_close || _sock != NULL) {
            return true;
        }

        _sock = new tcp::socket(_io_service);

        _sock->connect(*(ep.get()));

        Log::write(INFO, "Client connected to %s : %d\n",
                   ep->address().to_string().c_str(),
                   ep->port());

    } catch(...) {
        Log::write(ERROR, "Failed to connect to %s : %d\n",
                   ep->address().to_string().c_str(),
                    ep->port());

        return false;
    }

    if(!_recv_started) {
        try{
            // start recv thread
            _t_recv_respond =
                boost::thread(boost::bind(&Client::Connection::recvStart,this));

            _recv_started = true;

        }catch(exception& e){
            Log::write(ERROR, "Failed to start thread _t_recv_respond.\n");

            return false;
        }
    }

    return true;
}


Client::Connection::~Connection() {
}


void Client::Connection::recvStart() {
    Log::write(INFO, "Connection receiver thread started. <thread id : %ld>, <pid : %d> \n",
               (long int)syscall(SYS_gettid), getpid());

    while(waitForWork()) {

        Log::write(DEBUG, "Got calls in Connection\n");

        recvRespond();
    }

    close();

    Log::write(INFO, "Connection receiver thread exits.\n");
}


void Client::Connection::recvRespond() {

    int recv_call_id = -1;
    shared_ptr<Call> curr_call;

    try{
        size_t l = boost::asio::read(*(_sock),
            boost::asio::buffer(&recv_call_id, sizeof(recv_call_id)));

        if(l<=0) {
            Log::write(ERROR, "Failed to read call_id from connection <%s>\n",
                        this->toString().c_str());
            std::abort();
        }

        Log::write(DEBUG, "recv_call_id is %d\n",recv_call_id);

        _mutex_conn.lock();

        map<int,shared_ptr<Call>>::iterator iter= _calls.find(recv_call_id);

        if(iter == _calls.end()) {
            Log::write(ERROR, "Can not find received call id %d\n", recv_call_id);
            std::abort();
        }

        _mutex_conn.unlock();

        Log::write(DEBUG, "Found call for call id %d\n", recv_call_id);

        curr_call = iter->second;

        shared_ptr<Writable> val = Method::getNewInstance(curr_call->getValueClass());

        val->readFields(_sock);

        curr_call->setValue(val);

    }catch(exception& e){
        Log::write(ERROR, "Failed to read value for call %d and set value - %s\n",
                    curr_call->getId(), e.what());
    }
}


// this only returns when either:
// 1. should stop OR
// 2. get a call OR
// 3. wait for more than _max_wait_rep times.
// return - true : if get a call
//        - false: if should stop Or
//                 if wait for more than _max_wait_rep
bool Client::Connection::waitForWork() {
    int rep = 0;
    while(rep < _max_wait_rep) {

        std::unique_lock<std::mutex> ulock(_mutex_conn);

        if(_cond_conn.wait_for(ulock, chrono::milliseconds(_call_wait_time),
            [this] { return _calls.size() > 0; })) {

            // reset rep to 0, and start counting again.
            // this is to track last active time
            rep = 0;

            return true;

        } else if(_should_stop)
            return false;

        rep++;
    }

    // mark connection as closed, as we have been waiting and there is
    // no call comes
    markClosed();

    return false;
}


bool Client::Connection::addCall(shared_ptr<Call> call) {
    std::unique_lock<std::mutex> ulock(_mutex_conn);

    try{
        int curr_call_id = last_call_index++;

        call->setId(curr_call_id);

        _calls.insert(pair<int,shared_ptr<Call>>(curr_call_id,call));

        _cond_conn.notify_one();

        Log::write(DEBUG, "insert call %d to connection, _calls.size() %d\n",
                   last_call_index - 1, _calls.size());

    } catch(...) {
        Log::write(ERROR, "Failed to insert %s to _calls\n", call->toString().c_str());
        return false;
    }

    return true;
}


// ensure only 1 call is sending at a time.
void Client::Connection::sendCall(Call* call) {
    std::unique_lock<std::mutex> ulock(_mutex_send_call);

    if(!call->write()) {
        Log::write(ERROR, "Client::sendCall: Failed\n");
    }
}


void Client::Connection::markClosed() {
    bool falseVal = false;

    if(_should_close.compare_exchange_strong(falseVal, true)) {
        _cond_conn.notify_all();
    }
}


void Client::Connection::close() {
    if(_should_close) {

        // close socket
        boost::system::error_code ec;

        _sock->close(ec);

        delete _sock;

        // remove from client._connections
        _client->removeConnection(_ep);

        //clear calls
        cleanupCalls();

        Log::write(DEBUG, "!! Close Connection %d !!\n", index);
    } else {
        Log::write(INFO,
            "!! connection %d is not closed, coz _should_close is not set\n", index);
    }
}


// Connection is closing, erase all calls associated.
void Client::Connection::cleanupCalls() {
    std::unique_lock<std::mutex> ulock(_mutex_conn);

    map<int,shared_ptr<Call>>::iterator iter = _calls.begin();

    while(iter != _calls.end()){
        _calls.erase(iter->first);
        iter++;
    }

}
/// End of Connection


/// Client

Client::Client(){
    _last_connection_index = 0;
}


void Client::stop() {
    Log::write(INFO, "Client receiver finished\n");
}


shared_ptr<Writable> Client::call(shared_ptr<Writable> param,
                                  string v_class,
                                  shared_ptr<tcp::endpoint> ep) {

    shared_ptr<Call> call(new Call(param, v_class));
    shared_ptr<Connection> curr_conn;

    if((curr_conn = getConnection(ep, call)) == NULL) {
        Log::write(ERROR, "Can not connect to endpoint  %s : %d \n",
                   ep->address().to_string().c_str(),
                   ep->port());
        return NULL;
    }

    curr_conn->sendCall(call.get());

    while(!_should_stop && !call->getDone()) {
        call->wait(_call_wait_time);
    }

/// a connection won't be closed here. It's only closed when idle for a while.
//    // close the underline socket in this conn.
//    // a new socket will be created for a new conn.
//    curr_conn->close();

    return call->getValue();
}


shared_ptr<Client::Connection> Client::getConnection(shared_ptr<tcp::endpoint> ep,
                                                     shared_ptr<Call> call) {
    std::unique_lock<std::mutex> ulock(_mutex_client);

    shared_ptr<Client::Connection> conn;

    try{
        stringstream ss;
        ss<<ep->address().to_string()<<":"<<ep->port();

        map<string,shared_ptr<Client::Connection>>::iterator iter =
            _connections.find(ss.str());

        if(iter == _connections.end()) {
            conn = make_shared<Client::Connection>(ep, _last_connection_index++);

            _connections.insert(pair<string,shared_ptr<Client::Connection>>(ss.str(),conn));

            Log::write(DEBUG, "Create new connection %s\n",
                       conn->toString().c_str());

            conn->setClient(this);
        } else {
            conn = iter->second;

            Log::write(DEBUG, "Reuse connection %s\n",
                       conn->toString().c_str());
        }

        // only connect if it's not connected
        conn->connect(ep,call);

    }catch(exception& e){
        Log::write(ERROR, "Failed to retrieve/create connection : %s\n", e.what());

        return NULL;
    }

    if(!conn->addCall(call)) {
        Log::write(ERROR, "FATAL: can not insert call into _calls. is it full !?");
        return NULL;
    }

    call->setConnection(conn);

    return conn;
}


void Client::removeConnection(shared_ptr<tcp::endpoint> ep) {
    std::unique_lock<std::mutex> ulock(_mutex_client);

    stringstream ss;
    ss<<ep->address().to_string()<<":"<<ep->port();

    if(_connections.erase(ss.str())) {
        Log::write(ERROR, "Failed to erase connection to <%s:%d>\n",
                   ep->address().to_string().c_str(),
                   ep->port());
    } else {
        Log::write(INFO, "Erased connection to <%s:%d>\n",
                   ep->address().to_string().c_str(),
                   ep->port());
    }
}


Client::~Client()
{
}




