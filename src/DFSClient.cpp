#include "DFSClient.h"


DFSClient::DFSClient()
{
    ///TODO:pick up server ep from config

}


DFSClient::~DFSClient()
{
    //dtor
}


int DFSClient::create(string path, int replication, shared_ptr<Permission> perm) {

    try{

        shared_ptr<StringWritable> path_ = make_shared<StringWritable>(path);
        shared_ptr<IntWritable> rep_ = make_shared<IntWritable>(replication);

        shared_ptr<Writable> res = namenode.create(path_, rep_, perm);

        return ((IntWritable*)res.get())->get();

    }catch(...){
        Log::write(ERROR, "Failed to create file in name space - path %s, rep %d, perm %s\n",
                   path.c_str(), replication, perm->toString().c_str());

        return -1;
    }
}
