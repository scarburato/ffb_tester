//
// Created by dario on 20/07/20.
//

#ifndef FFB_TESTER_SERVER_HPP
#define FFB_TESTER_SERVER_HPP

#include <SDL_net.h>
#include "../common/inet_states.h"

class Server
{
private:
    TCPsocket mySocket;
    IPaddress myIp = {0};

    TCPsocket clientSocket;
public:
    Server();
    ~Server();

    ClientStatus readEvent();

    std::string readEffect();

    void sendEvent(ServerStatus status);
};


#endif //FFB_TESTER_SERVER_HPP
