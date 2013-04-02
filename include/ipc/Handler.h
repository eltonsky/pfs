#ifndef HANDLER_H
#define HANDLER_H

#include "BlockQueue.h"
#include "Call.h"
#include <boost/thread.hpp>
#include <sys/syscall.h>
#include "Method.h"
#include "Responder.h"

namespace Ipc{

    extern BlockQueue<std::shared_ptr<Call>> _bq_call;

    class Handler
    {
        public:
            bool _should_stop = false;

            Handler(short id);
            ~Handler();

            void start();

            void handle();

            void waitToFinish();

            inline void setResponder(Responder* resp) {
                _responder = resp;
            }

        protected:
        private:
            boost::thread _t_handler;
            short _handler_id;
            Responder* _responder;
    };
}

#endif // HANDLER_H
