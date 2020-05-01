#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
//
// Created by dario on 22/04/20.
//

#include <SDL.h>
#include <iostream>
#include "effect_set.hpp"
#include "common.hpp"

static void create_periodic(SDL_HapticPeriodic &effect, std::istream &typer = std::cin);
static void create_constant(SDL_HapticConstant &effect, std::istream &typer = std::cin);

void create_spring(SDL_HapticCustom periodic, std::istream &istream);

int create_effect(SDL_Haptic *haptic, uint16_t type, std::istream &typer)
{
    SDL_HapticEffect effect = {0};
    int effect_id, iterations;

    //typer >> iterations;

    effect.type = type;

    /** Common fields **/
    effect.periodic.direction.type = SDL_HAPTIC_CARTESIAN;
    typer >> effect.periodic.direction.dir[1];

    switch(type)
    {
        case SDL_HAPTIC_CONSTANT:
            create_constant(effect.constant, typer);
            break;
        case SDL_HAPTIC_SINE:
        case SDL_HAPTIC_TRIANGLE:
        case SDL_HAPTIC_SAWTOOTHUP:
        case SDL_HAPTIC_SAWTOOTHDOWN:
        //case SDL_HAPTIC_CUSTOM:
            create_periodic(effect.periodic, typer);
            break;
        case SDL_HAPTIC_SPRING:
            create_spring(effect.constant, typer);
            break;
        default:
            std::cerr << "Invalid!";
            exit(0xff);
    }

    effect_id = SDL_HapticNewEffect( haptic, &effect );
    if(effect_id == -1)
        THROW_SDL_ERROR(std::cerr, -1);

    SDL_Delay( 2000);

    return effect_id;
}

void play_effect(SDL_Haptic *haptic, int effect_id, unsigned int iterations)
{
    if(SDL_HapticRunEffect( haptic, effect_id, iterations ))
        THROW_SDL_ERROR(std::cerr, -1);
    SDL_Delay( 8000);
}

void destroy_effect(SDL_Haptic *haptic, int effect_id)
{
    SDL_HapticDestroyEffect(haptic, effect_id);
    //SDL_Delay( 3000);
}

void create_spring(SDL_HapticCondition &effect, std::istream &typer)
{
    // Commons
    typer >> effect.delay;
    typer >> effect.length;
    typer >> effect.right_sat[0];
    typer >> effect.left_sat[0];
    typer >> effect.right_coeff[0];
    typer >> effect.left_coeff[0];
    typer >> effect.deadband[0];
    typer >> effect.center[0];
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
    typer >> effect.phase;
    typer >> effect.offset;

    std::clog << "phase: " << effect.phase << '\n';
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

#pragma clang diagnostic pop