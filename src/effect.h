#ifndef EFFECT_H_
#define EFFECT_H_

#include "SDL.h"

class Effect {

    public:
        Effect();
        Effect(SDL_Renderer *sdl_renderer, float x, float y, int side);
        ~Effect();
        float x;
        float y;
        float dx;
        float dy;
        int life;
        SDL_Color color;
	    SDL_Texture *texture;
};

#endif