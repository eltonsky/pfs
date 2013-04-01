#ifndef DATANODEPROTOCOL_H
#define DATANODEPROTOCOL_H

#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <thread>
#include <memory>
#include "Log.h"
#include "RPC.h"

class DataNodeProtocol {
    public:

        DataNodeProtocol();
        virtual ~DataNodeProtocol();

        DataNodeProtocol(string server_host, int port){

            boost::system::error_code error;
            boost::asio::ip::address_v4 targetIP;
            targetIP = boost::asio::ip::address_v4::from_string(server_host.c_str(), error);

            _ep = make_shared<tcp::endpoint>();
            _ep->address(targetIP);
            _ep->port(port);
        }

        virtual shared_ptr<Writable> create(shared_ptr<IntWritable> Arg0,
                                            shared_ptr<IntWritable> Arg1,
                                            shared_ptr<IntWritable> Arg2,
                                            shared_ptr<IntWritable> Arg3) {

            return RPC::invoke("standard", _ep, "IntWritable", "NameNode", "create", Arg0, Arg1, Arg2, Arg3);
        }

        //...

    private:
        shared_ptr<tcp::endpoint> _ep;
};

#endif // DATANODEPROTOCOL_H
