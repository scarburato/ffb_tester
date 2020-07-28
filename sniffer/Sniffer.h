//
// Created by dario on 20/07/20.
//

#ifndef FFB_TESTER_SNIFFER_H
#define FFB_TESTER_SNIFFER_H

#include <pcap.h>
#include <iostream>
#include <list>
#include <thread>
#include <mutex>
#include <vector>

class Sniffer
{
public:
    struct Packet;
private:
    pcap_t *handle;            /* Session handle */
    char errbuf[PCAP_ERRBUF_SIZE] = {0};    /* Error string */

    std::thread captureThread;
    /** I know I should not use volatile :/ But CPU chaches are coerent and write true or false should be always an
     * atomic operation....
     */
    volatile bool prepareToStop = false;
    volatile bool store = false;
    volatile bool start = false;
    std::mutex packetsMutex;
    std::list<Packet> packets;

    void captureLoop();
public:
    struct Packet
    {
        uint8_t *content = nullptr;
        size_t length;

        Packet() = delete;
        Packet(const uint8_t *content, size_t length);
        __always_inline Packet(const Packet &p):
        Packet(p.content, p.length)
        {};

        __always_inline friend std::ostream &operator<<(std::ostream &printer, Sniffer::Packet const &packet1)
        {
            printer << "{ \"length\": " << packet1.length << ", \"bytes\": [";
            for (auto i = 0; i < packet1.length; i++)
            {
                if(i)
                    printer << ',';

                printer <<  (unsigned short) packet1.content[i] << ' ';
            }

            printer << "]}" << std::dec;
            return printer;
        }
    };

    __always_inline void capPackets(bool v) {store = v;};

    std::vector<Packet> getPackets();

    Sniffer() = delete;
    Sniffer(char const *dev);
    ~Sniffer();
};

#endif //FFB_TESTER_SNIFFER_H
