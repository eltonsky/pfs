#ifndef RESPONDER_H
#define RESPONDER_H

#include "BlockQueue.h"
#include "Call.h"
#include <boost/thread.hpp>
#include <sys/syscall.h>

using namespace std;

namespace Server{

    extern BlockQueue<shared_ptr<tcp::endpoint>> _bq_respond;
    const int _resp_wait_time = 100;

    class Responder
    {
        public:
            bool _should_stop = false;

            Responder();
            ~Responder();

            void start();

            void respond();

            void waitToFinish();

            bool waitForWork();

            bool doRespond(shared_ptr<Call>);

            bool doAsyncWrite();

            bool processResponse(shared_ptr<Connection>);

        protected:
        private:
            boost::thread _t_responder;

            std::mutex _mutex_resp;
            std::condition_variable _cond_resp;
    };

}
#endif // RESPONDER_H
