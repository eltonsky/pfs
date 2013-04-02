#ifndef NAMENODE_H
#define NAMENODE_H

#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <thread>
#include <memory>
#include "Log.h"
#include "DataNodeProtocol.h"
#include "ClientProtocol.h"
#include "ipc/Server.h"

class NameNode : public DataNodeProtocol, public ClientProtocol {
    public:

    NameNode(int rpcPort);
    ~NameNode();

    void start();

    void close();

    virtual shared_ptr<Writable> create(shared_ptr<StringWritable> path,
                                            shared_ptr<IntWritable> rep,
                                            shared_ptr<Permission> perm);

    //...

    private:
        Ipc::Server _rpcServer;


};

#endif // NAMENODE_H
