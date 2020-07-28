//
// Created by dario on 22/04/20.
//

#ifndef FFB_TESTER_COMMON_HPP
#define FFB_TESTER_COMMON_HPP

#undef main

#define THROW_SDL_ERROR(printer, exit_code) {        \
    printer << SDL_GetError() << std::endl;          \
    exit(exit_code);                                 \
}

#define THROW_SDL_ERROR(printer, exit_code) {        \
    printer << SDL_GetError() << std::endl;          \
    exit(exit_code);                                 \
}

#endif //FFB_TESTER_COMMON_HPP
