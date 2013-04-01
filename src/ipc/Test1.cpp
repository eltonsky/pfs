#include "Test1.h"

using namespace std;

Test1::Test1()
{
    //ctor
}

bool Test1::test1(string server_host, int port, int rep) {
    Log::write(INFO, "##Start thread %ld \n" , (long int)syscall(SYS_gettid));

    // get endponit
    boost::system::error_code error;
    boost::asio::ip::address_v4 targetIP;
    targetIP = boost::asio::ip::address_v4::from_string(server_host.c_str(), error);

    shared_ptr<tcp::endpoint> ep = make_shared<tcp::endpoint>();
    ep->address(targetIP);
    ep->port(port);

    //create client
    Client client;

    for(int i = 0 ; i < rep; i++) {
        //get param
        vector<shared_ptr<Writable>> params;
        params.push_back(shared_ptr<IntWritable>(new IntWritable(10)));
        params.push_back(shared_ptr<IntWritable>(new IntWritable(20)));
        MethodWritable* mw = new MethodWritable("FSNameSystem", "create", params);
        shared_ptr<Writable> w = shared_ptr<MethodWritable>(mw);

        shared_ptr<Writable> val = client.call(w, "IntWritable", ep);
        Log::write(INFO, "In thread %ld : %d #### CLIENT RESULT : %s\n" ,
                   (long int)syscall(SYS_gettid), i ,val->printToString().c_str());
    }

    return true;
}


bool Test1::test2(string server_host, int port, int thread, int rep) {
    vector<boost::thread> ts(thread);

    for(int i =0; i < thread; i++) {
        ts[i] = boost::thread(boost::bind(&Test1::test1, this, server_host, port, rep));
    }

    for(int i =0; i < thread; i++) {
        ts[i].join();
    }

    return true;
}


bool Test1::test10(string server_host, int port, int rep) {
    Log::write(INFO, "## test10 Start thread %ld \n" , (long int)syscall(SYS_gettid));

    // get endponit
    boost::system::error_code error;
    boost::asio::ip::address_v4 targetIP;
    targetIP = boost::asio::ip::address_v4::from_string(server_host.c_str(), error);

    shared_ptr<tcp::endpoint> ep = make_shared<tcp::endpoint>();
    ep->address(targetIP);
    ep->port(port);

    for(int i = 0 ; i < rep; i++) {
        //params
        shared_ptr<IntWritable> param1 = make_shared<IntWritable>(10);
        shared_ptr<IntWritable> param2 = make_shared<IntWritable>(20);

        shared_ptr<Writable> val = RPC::invoke("standard", ep, "IntWritable", "FSNameSystem", "create", param1, param2);

        Log::write(INFO, "In thread %ld : %d #### CLIENT RESULT : %s\n" ,
                   (long int)syscall(SYS_gettid), i ,val->printToString().c_str());

    }

    return true;
}


bool Test1::test20(string server_host, int port, int thread, int rep) {
    vector<boost::thread> ts(thread);

    for(int i =0; i < thread; i++) {
        ts[i] = boost::thread(boost::bind(&Test1::test10, this, server_host, port, rep));
    }

    for(int i =0; i < thread; i++) {
        ts[i].join();
    }

    return true;
}


Test1::~Test1()
{
    //dtor
}




