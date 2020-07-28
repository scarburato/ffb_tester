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
    int status;
    sockaddr clientAddress;
    socklen_t clientAddressLen;

    socketId = socket(AF_INET, SOCK_STREAM, 0);

    if (socketId == -1)
        throw std::runtime_error("Unable to open a socket!");

    address.sin_family = AF_INET,
            address.sin_port = htons(DEFAULT_SERVER_PORT);
    address.sin_addr.s_addr = INADDR_ANY;
    status = bind(socketId, (sockaddr *) &address, sizeof(sockaddr_in));
    if (status == -1)
        throw std::runtime_error("Unable to bind IP address");

    listen(socketId, 1);

    std::clog << "Waiting for a connection...\n";
    socketClient = accept(socketId, &clientAddress, &clientAddressLen);

    if (socketClient == -1)
        throw std::runtime_error("Error while accepting a connection");

    std::clog << "Opened connection to ";

    for (int i = 0; i < 4; i++)
        std::clog << (unsigned short) clientAddress.sa_data[i + 3] << '.';

    std::clog << std::endl;
}

ClientStatus Server::readEvent()
{
    uint16_t buffer = 0;
    read(socketClient, &buffer, packet_size);
    return (ClientStatus) buffer;
}

void Server::sendEvent(ServerStatus status)
{
    write(socketClient, &status, packet_size);
}

std::string Server::readEffect()
{
    char *buffer = nullptr;
    uint64_t size = 0;

    read(socketClient, &size, sizeof(uint64_t));

    buffer = new char[size];

    read(socketClient, buffer, size);


    std::string ret(buffer);
    delete buffer;
    return ret;
}
