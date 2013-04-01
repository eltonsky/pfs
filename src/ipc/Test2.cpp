#include "Test2.h"

Test2::Test2()
{
    //ctor
}

Test2::~Test2()
{
    //dtor
}


void Test2::test21(string server_host, int port, int rep) {

    DataNodeProtocol namenode(server_host, port);

    for(int i = 0 ; i < rep; i++) {
        shared_ptr<IntWritable> a1 = make_shared<IntWritable>(12);
        shared_ptr<IntWritable> a2 = make_shared<IntWritable>(23);
        shared_ptr<IntWritable> a3 = make_shared<IntWritable>(34);
        shared_ptr<IntWritable> a4 = make_shared<IntWritable>(45);

        shared_ptr<Writable> res = namenode.create(a1,a2,a3,a4);

        Log::write(INFO, "In thread %ld : %d #### CLIENT RESULT : %s\n" ,
                    (long int)syscall(SYS_gettid), i ,res->printToString().c_str());

    }
}


bool Test2::test20(string server_host, int port, int thread, int rep) {
    vector<boost::thread> ts(thread);

    for(int i =0; i < thread; i++) {
        ts[i] = boost::thread(boost::bind(&Test2::test21, this, server_host, port, rep));
    }

    for(int i =0; i < thread; i++) {
        ts[i].join();
    }

    return true;
}
