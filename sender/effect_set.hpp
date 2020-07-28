//
// Created by dario on 22/04/20.
//

#ifndef CMAKEDEMO_SINE_HPP
#define CMAKEDEMO_SINE_HPP

int create_effect(SDL_Haptic *haptic, uint16_t type, std::istream &typer, SDL_HapticEffect &effect);

/**
 * Creates an effect and returns its ID
 * @param haptic
 * @param type
 * @param typer
 * @return
 */
__always_inline int create_effect(SDL_Haptic *haptic, uint16_t type, std::istream &typer = std::cin)
{
    SDL_HapticEffect effect = {0};
    return create_effect(haptic, type, typer, effect);
}

/**
 * Reproduce an effect from is id
 * @param haptic
 * @param effect_id
 * @param iterations
 */
void play_effect(SDL_Haptic *haptic, int effect_id, unsigned int iterations = 1);

/**
 * Destroy an effect from its id
 * @param haptic
 * @param effect_id
 */
void destroy_effect(SDL_Haptic *haptic, int effect_id);

#endif //CMAKEDEMO_SINE_HPP
