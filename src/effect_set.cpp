//
// Created by dario on 22/04/20.
//

#include <SDL.h>
#include <iostream>
#include "effect_set.hpp"
#include "common.hpp"

static void create_periodic(SDL_HapticPeriodic &effect, std::istream &typer = std::cin);
static void create_constant(SDL_HapticConstant &effect, std::istream &typer = std::cin);

void create_effect(SDL_Haptic *haptic,uint16_t type, std::istream &typer)
{
    SDL_HapticEffect effect = {0};
    int effect_id;

    effect.type = type;

    /** Common fields **/
    effect.periodic.direction.type = SDL_HAPTIC_CARTESIAN;
    typer >> effect.periodic.direction.dir[0];

    std::clog << effect.type;
    switch(type)
    {
        case SDL_HAPTIC_CONSTANT:
            create_constant(effect.constant, typer);
            break;
        case SDL_HAPTIC_SINE:
            create_periodic(effect.periodic, typer);
            break;
        default:
            std::cerr << "Invalid!";
            exit(0xff);
    }

    std::cout << "Sono qua!\n";
    effect_id = SDL_HapticNewEffect( haptic, &effect );
    if(effect_id == -1)
        THROW_SDL_ERROR(std::cerr, -1);

    if(SDL_HapticRunEffect( haptic, effect_id, 1 ))
        THROW_SDL_ERROR(std::cerr, -1);
    SDL_Delay( 10050);
    SDL_HapticDestroyEffect(haptic, effect_id);
}

static void create_periodic(SDL_HapticPeriodic &effect, std::istream &typer)
{
    // Commons
    typer >> effect.delay;
    typer >> effect.length;
    typer >> effect.attack_length;
    typer >> effect.attack_level;
    typer >> effect.fade_length;
    typer >> effect.fade_level;

    typer >> effect.period;
    typer >> effect.magnitude;
    // offset
    // phase
}

static void create_constant(SDL_HapticConstant &effect, std::istream &typer)
{
    // Commons
    typer >> effect.delay;
    typer >> effect.length;
    typer >> effect.attack_length;
    typer >> effect.attack_level;
    typer >> effect.fade_length;
    typer >> effect.fade_level;

    typer >> effect.level;
}
