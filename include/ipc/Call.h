#ifndef CALL_H
#define CALL_H

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "Writable.h"
#include "Method.h"
#include <string>
#include "IntWritable.h"
#include "BlockQueue.h"
#include "Connection.h"

using namespace std;
using boost::asio::ip::tcp;

namespace Server{

    class Connection;

    extern const int recheck_interval;

    class Call
    {
        public:
            Call();
            Call(shared_ptr<Connection> conn);
            ~Call();
            bool read();
            bool write();

            string toString();

            // for test
            string print();

            inline const string getClass() const {return _class;}
            inline const string getMethod() const {return _method;}
            inline const vector<shared_ptr<Writable>> getParams() const {return _params;}

            inline void setValue(shared_ptr<Writable> v) {
                _strVal = v->toString();
                _value = v;
            }

            inline const string getValueStr() const {return _strVal;}
            inline const shared_ptr<Writable> getValue() const {return _value;}
            inline const tcp::socket* getSock() const {return _connection->getSock().get();}
            inline void setConnection(shared_ptr<Connection> conn) {_connection = conn;}
            inline shared_ptr<Connection> getConnection() const {return _connection;}
            inline void setId(int id) {_call_id = id;}
            inline int getId() {return _call_id;}
            inline void setPos(int pos) {_pos = pos;}
            inline int getPos() {return _pos;}

        protected:
        private:

            shared_ptr<Connection> _connection;
            int _call_id;
            string _class;
            string _method;
            vector<shared_ptr<Writable>> _params;
            shared_ptr<Writable> _value;
            int _pos = 0; // the pos to start writing again, for a async mode
            string _strVal;
    };

}

#endif // CALL_H
