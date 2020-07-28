//
// Created by dario on 20/07/20.
//

#ifndef FFB_TESTER_SERVER_HPP
#define FFB_TESTER_SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>

#include "../common/inet_states.h"

class Server
{
private:
    int socketId;
    sockaddr_in address;

    int socketClient;
public:
    Server();

    ClientStatus readEvent();

    std::string readEffect();

    void sendEvent(ServerStatus status);
};


#endif //FFB_TESTER_SERVER_HPP
