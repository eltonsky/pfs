#include "Responder.h"

namespace Ipc{

    const int max_repsond_size = 100;
    BlockQueue<shared_ptr<tcp::endpoint>> _bq_respond(max_repsond_size);


    Responder::Responder()
    {
        //ctor
    }


    void Responder::start() {
        _t_responder = boost::thread(boost::bind(&Ipc::Responder::respond, this));
    }


    void Responder::respond() {
        Log::write(INFO, "Responder started. <thread id : %ld>, <pid : %d> \n", (long int)syscall(SYS_gettid), getpid());

        while(!_should_stop) {

            shared_ptr<tcp::endpoint> ep = _bq_respond.pop();

            shared_ptr<Connection> conn;

            //lock
            _mutex_conns.lock();

            map<shared_ptr<tcp::endpoint>, shared_ptr<Connection>>::iterator iter =
                    _connections.find(ep);

            if (iter == _connections.end()) {
                _mutex_conns.unlock();

                Log::write(ERROR, "Failed to get conn to respond for <%s:%d>\n",
                           ep->address().to_string().c_str(), ep->port());

                std::abort();
            } else {
                conn = iter->second;
            }

            _mutex_conns.unlock();

            Log::write(DEBUG, "connection in responder is <%s:%d>\n",
                    ep->address().to_string().c_str(), ep->port());

            if(!processResponse(conn)) {
                Log::write(ERROR, "Failed to process a response from conn <%s:%d>\n",
                           ep->address().to_string().c_str(), ep->port());
                std::abort();
            }
        }
    }


    // this is called by multiple handlers. But each responder queue is synced
    // itself
    bool Responder::doRespond(shared_ptr<Call> call) {

        if(!call->getConnection()->respond_queue.try_push(call)) {
            Log::write(ERROR ,"FATAL: can not insert call into respond_queue. full !?\n");
            return false;
        }

        Log::write(DEBUG, "Connection %d respond_queue size is %d\n",
                   call->getConnection()->index,
                   call->getConnection()->respond_queue.size());

        // the mighty optimization to to skip _bq_respond and respond thread;
        // cause someconfusions! bear in mind with this!
        // need a configure to start or stop this one.
///// Comment this out for testing _bq_respond.
        if(call->getConnection()->respond_queue.size() == 1) {
            processResponse(call->getConnection());

            Log::write(DEBUG, "Optimize : respond from handler.\n");
        } else {
            // queue for connection index : which connection has a result
            // ready to be sent.
            if(!_bq_respond.try_push(call->getConnection()->getEndpoint())) {
                Log::write(ERROR,
                    "FATAL: can not insert call's connection endpoint into _bq_respond.\n");
                std::abort();
            }

            Log::write(INFO, "_bq_respond.size() %d\n", _bq_respond.size());
        }

        return true;
    }


    // conn: the respond queue of a connection having results ready.
    //
    bool Responder::processResponse(shared_ptr<Connection> conn) {

        shared_ptr<Call> call;

        if(!conn->respond_queue.try_pop(call)) {
            Log::write(ERROR, "Failed to pop a call from queue of Connection %d\n",
                       conn->index);
            return false;
        }

/// queue test for _bq_respond. For this purpose, the optimization above
/// can not be used.
//this_thread::sleep_for(chrono::milliseconds(50));

        //write some, for max throughput
        int res = conn->processResponse(call);

/// Don't close the connection, unless it's not being contacted for a while.
//        // close & remove conn after processing a call.
//        // coz a call is associated with a connection anyway, and a connection
//        // is useless afterwareds.
//        conn->close();
//
//        //remove from _connections
//        _mutex_conns.lock();
//
//        _connections.erase(conn->getEndpoint());
//
//        _mutex_conns.unlock();

        if(res < 0) {
            Log::write(ERROR, "Failed to process a call response <Connection:%d>, <call:%d>\n",
                       conn->index, call->getId());
            return false;
        } else if(res ==1) {
            // if haven't send all result data, add the call back to queue

            if(!_bq_respond.try_push(conn->getEndpoint(), false)) {
                Log::write(ERROR,
                    "FATAL: can not add call's connection endpoint back to _bq_respond. full !?\n");
                std::abort();
            }

            Log::write(INFO, "_bq_respond.size() %d\n", _bq_respond.size());
        }

        return true;
    }


    // not used. _bq_respond.pop() uses a condition_var which should works
    // the same.
    bool Responder::waitForWork() {
        std::unique_lock<std::mutex> ulock(_mutex_resp);

        while(true){

            if(_cond_resp.wait_for(ulock, chrono::milliseconds(_resp_wait_time),
                [this] { return _bq_respond.size() > 0; })) {

                if(_should_stop)
                    return false;
                else
                    return true;
            } else if(_should_stop)
                return false;
        }

    }


    void Responder::waitToFinish() {
        while(_bq_respond.size() != 0) {
            this_thread::sleep_for(chrono::milliseconds(recheck_interval));
        }

        //stop producing any more responds
        _should_stop = true;

        _t_responder.interrupt();

        Log::write(INFO, "responder finished\n");
    }


    Responder::~Responder()
    {
        //dtor
    }

}







