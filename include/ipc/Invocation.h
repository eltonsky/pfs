#ifndef INVOCATION_H
#define INVOCATION_H

#include <vector>
#include "Writable.h"
#include "MethodWritable.h"
#include "Client.h"


class Invocation
{
    public:
        Invocation();
        ~Invocation();

        // assume Param is a shared_ptr<Writable>
        template<typename Param1, typename ...ParamN>
        void getParams(Param1 p1, ParamN ... pn) {
            _params.push_back(p1);

            Log::write(DEBUG, "insert param <%s> into _prams\n", p1->printToString().c_str());

            getParams(pn...);
        }


        // for the last param
        template <typename Param>
        void getParams(Param p) {
            _params.push_back(p);

            Log::write(DEBUG, "insert last param <%s> into _prams\n", p->printToString().c_str());
        }


        template<typename Param1, typename ...ParamN>
        shared_ptr<Writable> invoke(shared_ptr<Client> client,
                                    shared_ptr<tcp::endpoint> ep,
                                    string valueClass,
                                    string className,
                                    string methodName,
                                    Param1 a1, ParamN ...an
                                    ) {

            if(client == NULL) {
                Log::write(ERROR, "Invocation::invoke : client is NULL\n");
                std::abort();
            }

            shared_ptr<Writable> val;

            try {
                // collect vector of params
                getParams(a1, an...);

                shared_ptr<MethodWritable> mw =
                        make_shared<MethodWritable>(className,
                                                     methodName,
                                                    _params);

                val = client->call(mw, valueClass, ep);

            }catch(exception& e) {
                Log::write(ERROR, "Invocation::invoke : Failed to make call from client\n");
                std::abort();
            }

            return val;
        }

    protected:
    private:
        vector<shared_ptr<Writable>> _params;

};

#endif // INVOCATION_H
