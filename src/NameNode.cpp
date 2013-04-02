#include "NameNode.h"


NameNode::NameNode(int port) : _rpcServer(port)
{
}


NameNode::~NameNode()
{
    //dtor
}


void NameNode::start() {
    _rpcServer.start();
}


shared_ptr<Writable> NameNode::create(shared_ptr<StringWritable> path,
                                            shared_ptr<IntWritable> rep,
                                            shared_ptr<Permission> perm) {

    int res = 12345;

    return make_shared<IntWritable>(res);
}


