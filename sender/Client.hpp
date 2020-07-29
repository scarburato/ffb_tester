//
// Created by dario on 22/07/20.
//

#ifndef FFB_TESTER_CLIENT_HPP
#define FFB_TESTER_CLIENT_HPP

#include <SDL_haptic.h>
#include <SDL_net.h>

#include "ostream_SDL.hpp"
#include "../common/inet_states.h"

class Client
{
private:
    TCPsocket socket;
    IPaddress serverIp = {0};
public:
    Client(const std::string &serverAddress);
    ~Client();

    void sendEvent(ClientStatus status);
    void sendEffect(const SDL_HapticEffect &);

    ServerStatus readEvent();
};


#endif //FFB_TESTER_CLIENT_HPP
