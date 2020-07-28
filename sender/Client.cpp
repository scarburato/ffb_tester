//
// Created by dario on 22/07/20.
//

#include <stdexcept>
#include <netdb.h>
#include <strings.h>
#include <zconf.h>
#include <sstream>
#include "Client.hpp"

Client::Client(const std::string &serverAddress)
{
    int status;

    socketId = socket(AF_INET, SOCK_STREAM, 0);

    if (socketId == -1)
        throw std::runtime_error("Unable to open a socket!");

    sockaddr_in temp = {0};
    temp.sin_family = AF_INET;
    temp.sin_port = htons(DEFAULT_SERVER_PORT);

    hostent *h = gethostbyname(serverAddress.c_str());

    if (!h)
        throw std::runtime_error("gethostname() failed");

    bcopy(h->h_addr, &temp.sin_addr, h->h_length);

    // Conessione
    status = connect(socketId, (sockaddr *) &(temp), sizeof(temp));

    if (status == -1)
        throw std::runtime_error("Unable to connect to the server");
}

Client::~Client()
{
    close(socketId);
}

ServerStatus Client::readEvent()
{
    uint16_t buffer = 0;
    read(socketId, &buffer, packet_size);
    return (ServerStatus) buffer;
}

void Client::sendEvent(ClientStatus status)
{
    write(socketId, &status, packet_size);
}

void Client::sendEffect(const SDL_HapticEffect &toSent)
{
    std::ostringstream ser;
    ser << toSent;
    std::string serString = ser.str();

    // Sent the length of the string
    uint64_t size = serString.length() + 1;
    write(socketId, &size, sizeof(uint64_t));

    // sent the string
    write(socketId, serString.c_str(), size);
}

