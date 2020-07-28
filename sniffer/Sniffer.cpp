//
// Created by dario on 20/07/20.
//

#include <stdexcept>
#include <cstring>
#include "Sniffer.h"

Sniffer::Packet::Packet(const uint8_t *cnt, const size_t l)
{
    length = l;
    content = new uint8_t [length];

    memcpy(content, cnt, length);
}

Sniffer::Sniffer(const char *dev)
{
    handle = pcap_open_live(dev, BUFSIZ, 1, 5000, errbuf);

    if (!handle)
        throw std::runtime_error(errbuf);

    int ret = pcap_setnonblock(handle, true, errbuf);

    if (ret == -1)
        throw std::runtime_error(errbuf);

    captureThread = std::thread(&Sniffer::captureLoop, this);
}

void Sniffer::captureLoop()
{
    pcap_pkthdr *header = nullptr;
    const uint8_t  *packet = nullptr;

    while(!prepareToStop)
    {
        int ret = pcap_next_ex(handle, &header, &packet);

        if(!store)
            continue;

        /** Error handling */
        switch (ret)
        {
        case 1: // Al 0k
            break;
        case PCAP_ERROR: // Errors
        case PCAP_ERROR_BREAK:
            throw std::runtime_error("Error in Sniffer::captureLoop, pcap_next_ex returned " + std::to_string(ret));
        case 0: // No packets this loop
        default:
            continue;
        }

        packetsMutex.lock();
        packets.emplace_back(packet, header->len);
        packetsMutex.unlock();
    }
}

Sniffer::~Sniffer()
{
    prepareToStop = true;
    captureThread.join();
    pcap_close(handle);
}

std::vector<Sniffer::Packet> Sniffer::getPackets()
{
    packetsMutex.lock();
    std::vector<Packet> resultSet;

    for(const auto& packet: packets)
        resultSet.push_back(packet);

    packets.clear();

    packetsMutex.unlock();
    return resultSet;
}
