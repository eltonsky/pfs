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

class NameNode : DataNodeProtocol{
    public:

    NameNode();
    ~NameNode();

    virtual shared_ptr<Writable> create(shared_ptr<IntWritable> Arg0,
                                        shared_ptr<IntWritable> Arg1,
                                        shared_ptr<IntWritable> Arg2,
                                        shared_ptr<IntWritable> Arg3) {
        int res = Arg0->get() + Arg1->get() + Arg2->get() + Arg3->get();

        return make_shared<IntWritable>(res);
    }

    //...

};

#endif // NAMENODE_H
