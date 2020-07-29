#include <iostream>
#include <SDL.h>
#include <list>
#include <string>
#include "Client.hpp"
#include "common.hpp"
#include "effect_set.hpp"

int main(int argc, char *argv[])
{
    char *serverAddr = nullptr;

    int sel = 0, n_joysticks = 0, status;
    SDL_Joystick *joy = nullptr;
    SDL_Haptic *haptic = nullptr;
    uint16_t effect_code = 0;
    uint16_t repeat = 1;
    uint8_t gain = 100, autocenter = 0;

    std::list<int> effects;
    bool run = true;
    // Chek if I have to skip play
    for (int i = 1; i < argc; i++)
    {
        const std::string arg(argv[i]);
        if (arg == "-s" || arg == "--skip-play")
            run = false;
        else if (arg[0] == '-' && arg[1] == 'r')
            repeat = stoi(arg.substr(2, 5));
        else if (arg[0] == '-' && arg[1] == 'g')
            gain = stoi(arg.substr(2, 5));
        else if (arg[0] == '-' && arg[1] == 'c')
            autocenter = stoi(arg.substr(2, 5));
        else if (arg[0] == '-' && arg[1] == 'S')
            serverAddr = argv[i] + 2;
    }

    std::clog << serverAddr << '\n';
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "Error initializing SDL!\n";
        return 1;
    }
    if(SDLNet_Init() == -1)
    {
        std::cerr << "Error initializing SDL_net!\n";
        return 1;
    }

    Client client(serverAddr ? serverAddr : "127.0.0.1");

    // Enable exc
    //std::cin.exceptions (std::istream::failbit);


    n_joysticks = SDL_NumJoysticks();
    if (!n_joysticks)
    {
        std::cerr << "No joysticks attached!\n";
        return 1;
    }

    // List joysticks
    for (int i = 0; i < n_joysticks; i++)
        std::cout << i << "] " << SDL_JoystickNameForIndex(i) << '\n';

    std::cout << "? ";
    std::cin >> sel;

    joy = SDL_JoystickOpen(sel);
    if (!joy)
    THROW_SDL_ERROR(std::cerr, 2);

    status = SDL_JoystickIsHaptic(joy);
    if (status == SDL_FALSE)
    {
        std::cerr << "Forcefeedback not supported :/\n";
        return 3;
    }
    if (status != SDL_TRUE)
    THROW_SDL_ERROR(std::cerr, 3);

    // Open the device
    haptic = SDL_HapticOpenFromJoystick(joy);
    if (!haptic)
    THROW_SDL_ERROR(std::cerr, 4);

    // bitmask of supported effect
    unsigned int effectsSupported = SDL_HapticQuery(haptic);
    std::cout << "Supported effects: " << std::hex << effectsSupported << std::dec << '\n';

    // Now set gain to the maximun
    //if (SDL_HapticSetGain(haptic, gain))
    //THROW_SDL_ERROR(std::cerr, 5);

    // Read effect, only one for the test!
    std::cin >> effect_code;

    if ((SDL_HapticQuery(haptic) & effect_code) == 0)
    {
        std::cerr << "The effected is not supported :/\n";
        return 6;
    }

    std::cin.ignore(256, '\n');
    std::cin.ignore(256, '\n');
    while (!std::cin.eof())
    {
        SDL_HapticEffect effect = {0};

        client.sendEvent(UPLOAD_START);
        if(client.readEvent() != READY)
            throw std::runtime_error("Server didn't reply correctly");

        effects.push_back(create_effect(haptic, effect_code, std::cin, effect));

        client.sendEvent(UPLOAD_FINISH);
        if(client.readEvent() != READY)
            throw std::runtime_error("Server didn't reply correctly");

        client.sendEffect(effect);

        std::cin.ignore(256, '\n');
    }

    // Playing
    if (run)
    {
        client.sendEvent(PLAY);
        for (int i : effects)
        {
            std::clog << "Now playing effect n° " << i << std::endl;
            play_effect(haptic, i, repeat);
        }
    }

    // Closing
    client.sendEvent(DESTROY);
    for (int i : effects)
        destroy_effect(haptic, i);

    // Closing
    SDL_HapticClose(haptic);
    SDL_JoystickClose(joy);

    client.sendEvent(CLIENT_LEAVE);
    std::cout << "Goodbye!" << std::endl;

    return 0;
}