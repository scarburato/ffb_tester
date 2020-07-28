#include <iostream>
#include "Sniffer.h"
#include "Server.hpp"

int main(int argc, char *argv[])
{
    Sniffer usbmon(argc >= 2 ? argv[1] : "usbmon0");
    Server server;

    bool firstEvent = true;
    std::cout << "[";
    while (true)
    {
        ClientStatus op = server.readEvent();
        std::vector<Sniffer::Packet> packets;
        bool firstPacket = true;

        switch (op)
        {
        case UPLOAD_START:
            usbmon.capPackets(true);
            std::clog << "Start capping\n";
            server.sendEvent(READY);
            break;

        case UPLOAD_FINISH:
            usbmon.capPackets(false);
            std::clog << "Stop capping\n";
            packets = usbmon.getPackets();
            server.sendEvent(READY);
            if(firstEvent)
                firstEvent = false;
            else
                std::cout << ',';

            std::cout << "{ \"effect\": " << server.readEffect() << ",\n \"packets\": [";

            firstPacket = true;
            for(const auto& packet : packets)
            {
                if(firstPacket)
                    firstPacket = false;
                else
                    std::cout << ',';

                std::cout << packet;
            }
            std::cout << "]}\n";

            break;
        case PAUSE:
            break;
        case CLIENT_LEAVE:
            std::cout << "]";
            std::clog << "Closing....\n";
            return 0;
        default:
            std::cerr << "Unhandled " << op << '\n';
        }
        //std::cout << statesStrings[op] << usbmon.next() << '\n';
    }
}