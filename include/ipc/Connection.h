#ifndef CONNECTION_H
#define CONNECTION_H

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <sys/time.h>
#include "Writable.h"
#include "Method.h"
#include <string>
#include "IntWritable.h"
#include "BlockQueue.h"
#include "Utils.h"

namespace Server {
    class Call;

    extern int _last_connection_index;
    extern std::mutex _mutex_conns;
    extern std::condition_variable _cond_conns;

    // clean up interval
    extern long conn_cleanup_interval;
    extern int num_connections;

    class Connection{

        shared_ptr<tcp::socket> _sock;
        shared_ptr<tcp::endpoint> _ep;
        std::mutex _mutex;
        const static int _max_respond_size = 100;
        long _last_contact = -1;
        const static long _max_idle_duration = 500;

    public:
        BlockQueue<shared_ptr<Call>> respond_queue;
        int index;

        Connection(shared_ptr<tcp::socket> sock,
                   shared_ptr<tcp::endpoint> ep, int i);

        int processResponse(shared_ptr<Call>);

        void close();

        inline string toString() {
            stringstream ss;
            ss<<"<"<<_ep->address().to_string()<< " : "<< _ep->port()<<">";
            return ss.str();
        }


        inline shared_ptr<tcp::socket> getSock() const{
            return _sock;
        }


        inline shared_ptr<tcp::endpoint> getEndpoint() const {
            return _ep;
        }


        inline void setLastContact() {
            _last_contact = Utils::getTime();
        }


        inline bool timeOut() {
            long current = Utils::getTime();

            return ((current - _last_contact) > _max_idle_duration);
        }
    };

    // delcare map after definition of connection map.
    extern map<shared_ptr<tcp::endpoint>, shared_ptr<Connection>> _connections;

}

#endif // CONNECTION_H
