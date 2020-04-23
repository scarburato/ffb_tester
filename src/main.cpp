#include <SDL.h>
#include <iostream>

#include "common.hpp"
#include "effect_set.hpp"

int main()
{
    int sel = 0, n_joysticks = 0, status;
    SDL_Joystick *joy = nullptr;
    SDL_Haptic *haptic = nullptr;
    SDL_HapticEffect effect = {0};
    uint16_t effect_code = 0;

    // Enable exc
    std::cin.exceptions (std::istream::failbit);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
        std::cerr << "Error initializing SDL!\n";
        return 1;
    }
    n_joysticks = SDL_NumJoysticks();
    if(!n_joysticks)
    {
        std::cerr << "No joysticks attached!\n";
        return 1;
    }

    // List joysticks
    for(int i = 0; i < n_joysticks; i++)
        std::cout << i << "] " << SDL_JoystickNameForIndex(i) << '\n';

    std::cout << "? ";
    std::cin >> sel;

    joy = SDL_JoystickOpen(sel);
    if(!joy)
        THROW_SDL_ERROR(std::cerr, 2);

    status = SDL_JoystickIsHaptic(joy);
    if(status == SDL_FALSE)
    {
        std::cerr << "Forcefeedback not supported :/\n";
        return 3;
    }
    if(status != SDL_TRUE)
        THROW_SDL_ERROR(std::cerr, 3);

    // Open the device
    haptic = SDL_HapticOpenFromJoystick( joy );
    if (!haptic)
        THROW_SDL_ERROR(std::cerr, 4);

    // Read effect, only one for the test!
    std::cin >> effect_code;
    std::cout << effect_code;
    /*if(!(effect_code && !(effect_code & (effect_code-1))))
    {
        std::cerr << "I can only support 1 effect at the time :(\n";
        return 5;
    }*/

    if((SDL_HapticQuery(haptic) & effect_code)==0)
    {
        std::cerr << "The effected is not supported :/\n";
        return 6;
    }

    std::cin.ignore(256, '\n');
    std::cin.ignore(256, '\n');
    while(! std::cin.eof())
    {
        create_effect(haptic, effect_code, std::cin);
    }

    // Closing
    SDL_HapticClose(haptic);
    SDL_JoystickClose(joy);

    std::cout << "Goodbye!" << std::endl;

    return 0;
}

#pragma clang diagnostic pop