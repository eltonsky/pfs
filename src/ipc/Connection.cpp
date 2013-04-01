#include "Connection.h"
#include "Call.h"

namespace Server{

    int _last_connection_index;
    std::mutex _mutex_conns;
    std::condition_variable _cond_conns;
    map<shared_ptr<tcp::endpoint>, shared_ptr<Connection>> _connections;
    long conn_cleanup_interval = 400;
    int num_connections = 0;

    Connection::Connection(shared_ptr<tcp::socket> sock,
                   shared_ptr<tcp::endpoint> ep, int i) :
                   _sock(sock), _ep(ep),
                   respond_queue(_max_respond_size), index(i) {

        //set last contact in milli secs
        setLastContact();

    }


    // write in a async fasion:
    // add the call back to queue if it's not fully sent
    int Connection::processResponse(shared_ptr<Call> call) {

        try {
            std::unique_lock<std::mutex> ulock(_mutex);

            tcp::socket* sock = _sock.get();

            size_t wrote = 0;
            size_t length = call->getValueStr().length();
            size_t curr_pos = call->getPos();
            size_t left = length - curr_pos;

            if(curr_pos == 0) {
                int call_id = call->getId();
                //assume call_id is always sent without prob.
                boost::asio::write(*(sock), boost::asio::buffer((const char*)&call_id, sizeof(call_id)));

                boost::asio::write(*(sock), boost::asio::buffer((const char*)&left, sizeof(left)));
            } else {

                if( curr_pos < 0 || curr_pos > length) {
                    Log::write(ERROR ,"Illegal pos value (%d) for call %d\n", curr_pos, call->getId());
                    return -1;
                }
            }

            const char* start = call->getValueStr().c_str() + curr_pos;

            Log::write(DEBUG,
                       "Connection::processResponse : call id %d, length %d, curr_pos %d, value %s\n",
                       call->getId(), length, curr_pos, call->getValue()->printToString().c_str());

            wrote = sock->write_some(boost::asio::buffer(start, left));

            Log::write(DEBUG, "wrote is %d, left is %d\n", wrote, left);

            // if didn't write all data, add the call back to respond_queue
            // and move on to next connection.
            if(wrote < left) {
                call->setPos(wrote+curr_pos);

                if(!respond_queue.try_push(call)) {
                    Log::write(ERROR ,"FATAL: can not insert call into respond_queue. full !?\n");
                    return -1;
                }

                return 1;
            }

            return 0;

        } catch(exception& e){
            Log::write(ERROR, "Failed to process call in connection %d\n",index);
            return -1;
        }
    }


    void Connection::close() {
        Log::write(DEBUG, "current _sock ref count %d\n", _sock.use_count());

        if(_sock.get() != NULL)
            _sock->close();
    }

}




