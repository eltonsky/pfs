#ifndef LISTENER_H
#define LISTENER_H

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <sys/syscall.h>
#include <vector>
#include <queue>
#include <memory>
#include <semaphore.h>
#include <csignal>
#include "BlockQueue.h"
#include "Call.h"
#include "Handler.h"

using boost::asio::ip::tcp;

namespace Server {

class Listener
{
    class Reader;

    public:
        bool _should_stop = false;
        static const short num_readers = 2;
        const short max_reader_queue_size = 100;
        boost::asio::io_service _io_service_listener;
        boost::asio::io_service _io_service_disp;
        int max_accept_conns;

        long last_cleanup_time = 0;

        Listener(int port);

        // remove virtual from all pipeline classes,
        // coz they will not be inherited anyway.
        ~Listener();

        void run();

        void start();

        void stop();

        void join();

        void cleanup_connections();

        void close_connection(shared_ptr<Connection>,
                                    shared_ptr<tcp::endpoint>);

        void handle_read(shared_ptr<tcp::socket> sock,
                         shared_ptr<tcp::endpoint> ep,
                         const boost::system::error_code& error,
                            size_t bytes_transferred);

        void handle_accept(shared_ptr<tcp::socket> sock,
                           shared_ptr<tcp::endpoint> ep,
                            const boost::system::error_code& error);

        bool addToReader(shared_ptr<Connection> conn);

    private:

        int _port;
        int _last_reader_index;

        tcp::acceptor _acceptor;
        std::shared_ptr<tcp::socket> _curr_sock;

        boost::thread _t_listener;

        shared_ptr<Reader> _readers[num_readers];

        void _do_accept();

        //Reader
        class Reader {

            BlockQueue<shared_ptr<Connection>> _bq_conn;
            boost::thread _t_reader;
            const short _reader_index;

            Listener* _listener;

            void _do_read(shared_ptr<Connection> conn) {

                Log::write(INFO, "_do_read from reader %d\n", _reader_index);

                try{
                    //try to read a full call object
                    shared_ptr<Call> s_call = make_shared<Call>(conn);

                    if(!s_call->read()){
                        Log::write(ERROR, "Failed to read call in reader %d from conn %s\n",
                                   _reader_index, conn->toString().c_str());

                        // close connection
                        _listener->close_connection(conn, conn->getEndpoint());

                        return;
                    }

                    // set conn contacted timestamp
                    conn->setLastContact();

                    Log::write(DEBUG, "Reader %d get call %s\n",
                               _reader_index, s_call->toString().c_str());

                    if(!_bq_call.try_push(s_call)) {
                        Log::write(ERROR, "FATAL: can not insert call into _bq_call!\n");
                        std::abort();
                    }

                    Log::write(INFO, "_bq_call.size() %d\n", _bq_call.size());

                    //put this conn back to listener, in case client sends another call.
                    conn->getSock()->async_read_some(boost::asio::null_buffers(),
                        boost::bind(&Listener::handle_read, _listener,
                                      conn->getSock(), conn->getEndpoint(),
                                      boost::asio::placeholders::error,
                                      boost::asio::placeholders::bytes_transferred));

                } catch (exception& e) {
                    Log::write(ERROR, e.what());
                }
            }

        public:

            bool _should_stop = false;

            Reader(short index, short max_q_size) :
                _bq_conn(max_q_size),
                _reader_index(index){
            }

            ~Reader(){}

            void join() {
                _t_reader.join();
            }

            void run() {
                _t_reader = boost::thread(boost::bind(&Server::Listener::Reader::start, this));
            }

            void start() {
                Log::write(INFO, "Reader %d started. <thread id : %ld>, <pid : %d> \n",
                       _reader_index, (long int)syscall(SYS_gettid), getpid());

                while(!_should_stop) {

                    _do_read(_bq_conn.pop());
                }
            }

            bool add(shared_ptr<Connection> conn) {
                bool res = _bq_conn.try_push(conn);

                Log::write(DEBUG, "_bq_conn 's size is %d\n", _bq_conn.size());

                return res;
            }

            void waitToFinish() {
                while(_bq_conn.size() != 0) {
                    this_thread::sleep_for(chrono::milliseconds(recheck_interval));
                }

                Log::write(INFO, "Reader %d finished\n", _reader_index);

                // stop producing any more calls
                _should_stop = true;

                _t_reader.interrupt();
            }


            inline void setListener(Listener* l) { _listener = l; }
            inline Listener* getListener() { return _listener;}

        };

};


}

#endif // LISTENER_H
