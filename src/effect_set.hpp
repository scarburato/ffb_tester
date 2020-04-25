//
// Created by dario on 22/04/20.
//

#ifndef CMAKEDEMO_SINE_HPP
#define CMAKEDEMO_SINE_HPP

int create_effect(SDL_Haptic *haptic,uint16_t type, std::istream &typer = std::cin);
void play_effect(SDL_Haptic *haptic, int effect_id, unsigned int iterations = 1);
void destroy_effect(SDL_Haptic *haptic, int effect_id);

#endif //CMAKEDEMO_SINE_HPP
