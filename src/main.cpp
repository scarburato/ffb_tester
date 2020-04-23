#include <SDL.h>
#include <iostream>

#include "common.hpp"
#include "effect_set.hpp"

int test_haptic( SDL_Joystick *joystick );
int main()
{
    int sel = 0, n_joysticks = 0, status;
    SDL_Joystick *joy = nullptr;
    SDL_Haptic *haptic = nullptr;
    SDL_HapticEffect effect = {0};
    uint16_t effect_code = 0;

    if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER) < 0){
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
    /*if(!(effect_code && !(effect_code & (effect_code-1))))
    {
        std::cerr << "I can only support 1 effect at the time :(\n";
        return 5;
    }*/

    status = SDL_HapticEffectSupported(haptic, &effect);
    if(status == SDL_FALSE)
    {
        std::cerr << "The effected is not supported :/\n";
        return 6;
    }
    if(status != SDL_TRUE)
        THROW_SDL_ERROR(std::cerr, 6);

    while(! std::cin.eof())
    {
        create_effect(haptic, effect_code, std::cin);
    }

    // Closing
    SDL_HapticClose(haptic);
    SDL_JoystickClose(joy);

    return 0;
}

int test_haptic( SDL_Joystick *joystick ) {
    SDL_Haptic *haptic;
    SDL_HapticEffect effect;
    int effect_id;

    // Open the device
    haptic = SDL_HapticOpenFromJoystick( joystick );
    if (haptic == NULL) return -1; // Most likely joystick isn't haptic

    // See if it can do sine waves
    if ((SDL_HapticQuery(haptic) & SDL_HAPTIC_SINE)==0) {
        SDL_HapticClose(haptic); // No sine effect
        return -2;
    }

    // Create the effect
    SDL_memset( &effect, 0, sizeof(SDL_HapticEffect) ); // 0 is safe default
    effect.type = SDL_HAPTIC_SINE;
    effect.periodic.direction.type = SDL_HAPTIC_CARTESIAN; // Polar coordinates
    effect.periodic.direction.dir[0] = 1; // Force comes from south
    effect.periodic.period = 1000; // 1000 ms
    effect.periodic.magnitude = 20000; // 20000/32767 strength
    effect.periodic.length = 2500; // 5 seconds long
    effect.periodic.attack_length = 1000; // Takes 1 second to get max strength
    effect.periodic.fade_length = 1000; // Takes 1 second to fade away

    // Upload the effect
    effect_id = SDL_HapticNewEffect( haptic, &effect );

    // Test the effect
    SDL_HapticRunEffect( haptic, effect_id, 1 );
    SDL_Delay( 5000); // Wait for the effect to finish

    // We destroy the effect, although closing the device also does this
    SDL_HapticDestroyEffect( haptic, effect_id );

    // Close the device
    SDL_HapticClose(haptic);

    return 0; // Success
}

#pragma clang diagnostic pop