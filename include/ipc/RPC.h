#ifndef RPC_H
#define RPC_H

#include "Invocation.h"
#include <vector>
#include <mutex>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "Writable.h"
#include "MethodWritable.h"
#include "Client.h"

using namespace std;
using namespace boost::asio::ip;

class RPC
{
    protected:
    private:
        RPC();
        RPC(const RPC&);

        class ClientCache {
            map<string,shared_ptr<Client>> _clients;
            const int _max_num = 100;
            std::mutex _mutex;

            public:
                ClientCache(){}
                ~ClientCache(){}

                shared_ptr<Client> getClient(string sockType) {
                    lock_guard<mutex> lg(_mutex);

                    shared_ptr<Client> client;

                    map<string, shared_ptr<Client>>::iterator iter =
                    _clients.find(sockType);

                    if (iter == _clients.end()) {
                        client = make_shared<Client>();

                        _clients.insert(pair<string,shared_ptr<Client>>(sockType, client));

                        Log::write(DEBUG, "Create a new client.\n");

                    } else {
                        client = iter->second;

                        Log::write(DEBUG, "Reuse exist client.\n");
                    }

                    return client;
                }

        };

        static ClientCache _clientCache;


    public:
        template<typename Param1, typename ...ParamN>
        static shared_ptr<Writable> invoke(string sockType,
                                    shared_ptr<tcp::endpoint> ep,
                                    string valueClass,
                                    string className,
                                    string methodName,
                                    Param1 a1, ParamN ...an){

            shared_ptr<Client> client = _clientCache.getClient(sockType);

            // create a new Invocation obj each call, otherwise all threads
            // will race Invocation._params
            Invocation invoc;
            shared_ptr<Writable> res = invoc.invoke(client,
                    ep,
                    valueClass,
                    className,
                    methodName,
                    a1, an...);

            return res;
        }

};

#endif // RPC_H
