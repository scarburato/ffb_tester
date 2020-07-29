//
// Created by dario on 20/07/20.
//

#include <stdexcept>
#include <iostream>
#include <zconf.h>
#include <netdb.h>
#include "Server.hpp"

Server::Server()
{
    int ret;

    ret = SDLNet_ResolveHost(&myIp, nullptr,DEFAULT_SERVER_PORT);
    if(ret == -1)
        throw std::runtime_error(SDLNet_GetError());

    mySocket = SDLNet_TCP_Open(&myIp);
    if(!mySocket)
        throw std::runtime_error(SDLNet_GetError());

    std::clog << "Waiting for connection...\n";
    do
    {
        clientSocket = SDLNet_TCP_Accept(mySocket);
        usleep(1000);
    }
    while (!clientSocket);

    std::clog << "Connection opened\n";
    //if(!clientSocket)
    //    throw std::runtime_error(SDLNet_GetError());
}

Server::~Server()
{
    SDLNet_TCP_Close(clientSocket);
    SDLNet_TCP_Close(mySocket);
}

ClientStatus Server::readEvent()
{
    uint16_t buffer = 0;
    int ret = SDLNet_TCP_Recv(clientSocket, &buffer, packet_size);

    if(ret <= 0)
        throw std::runtime_error(SDL_GetError());

    return (ClientStatus) buffer;
}

void Server::sendEvent(ServerStatus status)
{
    int ret = SDLNet_TCP_Send(clientSocket, &status, packet_size);
    if(ret <= 0)
        throw std::runtime_error(SDL_GetError());
}

std::string Server::readEffect()
{
    char *buffer = nullptr;
    uint64_t size = 0;
    int code;

    code = SDLNet_TCP_Recv(clientSocket, &size, sizeof(uint64_t));
    if(code <= 0)
        throw std::runtime_error(SDL_GetError());

    buffer = new char[size];
    code = SDLNet_TCP_Recv(clientSocket, buffer, size);
    if(code <= 0)
        throw std::runtime_error(SDL_GetError());

    std::string ret(buffer);
    delete buffer;
    return ret;
}
