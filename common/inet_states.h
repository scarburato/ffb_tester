//
// Created by dario on 21/07/20.
//

#ifndef FFB_TESTER_INET_STATES_H
#define FFB_TESTER_INET_STATES_H

#include <cstdint>

#define DEFAULT_SERVER_PORT 9123

const unsigned packet_size = sizeof(uint16_t);

enum ServerStatus
{
    READY = 0,
    CLOSING
};

enum ClientStatus
{
    UPLOAD_START = 0,
    UPLOAD_FINISH,
    PLAY,
    STOP,
    DESTROY,

    PAUSE = 0xff,

    CLIENT_LEAVE = 0xff00
};

char const *const statesStrings[] = {
        "effect_upload",
        "effect_play",
        "effect_stop"
};

#endif //FFB_TESTER_INET_STATES_H
