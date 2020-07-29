//
// Created by dario on 22/07/20.
//

#include <stdexcept>
#include <sstream>
#include "Client.hpp"

Client::Client(const std::string &serverAddress)
{
    int ret;

    ret = SDLNet_ResolveHost(&serverIp, serverAddress.c_str(), DEFAULT_SERVER_PORT);
    if(ret == -1)
        throw std::runtime_error(SDLNet_GetError());

    socket = SDLNet_TCP_Open(&serverIp);
    if(!socket)
        throw std::runtime_error(SDLNet_GetError());
}

Client::~Client()
{
    SDLNet_TCP_Close(socket);
}

ServerStatus Client::readEvent()
{
    uint16_t buffer = 0;
    SDLNet_TCP_Recv(socket, &buffer, packet_size);
    return (ServerStatus) buffer;
}

void Client::sendEvent(ClientStatus status)
{
    SDLNet_TCP_Send(socket, &status, packet_size);
}

void Client::sendEffect(const SDL_HapticEffect &toSent)
{
    std::ostringstream ser;
    ser << toSent;
    std::string serString = ser.str();

    // Sent the length of the string
    uint64_t size = serString.length() + 1;
    SDLNet_TCP_Send(socket, &size, sizeof(uint64_t));

    // sent the string
    SDLNet_TCP_Send(socket, serString.c_str(), size);
}

