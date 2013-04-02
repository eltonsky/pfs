#ifndef CLIENTPROTOCOL_H
#define CLIENTPROTOCOL_H

#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <thread>
#include <memory>
#include "Log.h"
#include "RPC.h"
#include "IntWritable.h"
#include "StringWritable.h"
#include "Permission.h"

class ClientProtocol {
    public:

        ClientProtocol();
        ClientProtocol(string server_host, int port);
        virtual ~ClientProtocol();

        virtual shared_ptr<Writable> create(shared_ptr<StringWritable> path,
                                            shared_ptr<IntWritable> rep,
                                            shared_ptr<Permission> perm);

        //...

    private:
        shared_ptr<tcp::endpoint> _ep;
};

#endif // CLIENTPROTOCOL_H
