#ifndef SERVER_H
#define SERVER_H

#include "Listener.h"
#include "Handler.h"
#include "Responder.h"

namespace Ipc{

    const short num_handlers = 2;

    class Server
    {
        public:
            Server(int);
            ~Server();

            void start();

            void stop();

        protected:
        private:
            Listener _listener;
            shared_ptr<Handler> _handlers[num_handlers];
            Responder _responder;
    };

}

#endif // SERVER_H
