#include "ClientProtocol.h"

ClientProtocol::ClientProtocol()
{
    //ctor
}


ClientProtocol::ClientProtocol(string server_host, int port){
    boost::system::error_code error;
    boost::asio::ip::address_v4 targetIP;
    targetIP = boost::asio::ip::address_v4::from_string(server_host.c_str(), error);

    _ep = make_shared<tcp::endpoint>();
    _ep->address(targetIP);
    _ep->port(port);
}


ClientProtocol::~ClientProtocol()
{
    //dtor
}


shared_ptr<Writable> ClientProtocol::create(shared_ptr<StringWritable> path,
                                            shared_ptr<IntWritable> rep,
                                            shared_ptr<Permission> perm) {

    return RPC::invoke("standard", _ep, "IntWritable", "NameNode", "create",
                       path, rep, perm);
}


