#include "Server.h"

namespace Ipc {

    Server::Server(int port) : _listener(port), _responder()
    {
        for(short i=0;i<num_handlers;i++) {
            _handlers[i] = make_shared<Handler>(i);
            _handlers[i]->setResponder(&_responder);
        }
    }


    void Server::start() {
        _responder.start();

        for(short i=0;i<num_handlers;i++) {
            _handlers[i].get()->start();
        }

        _listener.run();
    }


    void Server::stop() {
        _listener.stop();

        for(short i=0;i<num_handlers;i++) {
            _handlers[i].get()->waitToFinish();
        }

        _responder.waitToFinish();
    }


    Server::~Server()
    {
        //dtor
    }

}
