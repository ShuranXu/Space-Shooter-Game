#ifndef _BULLET_H_
#define _BULLET_H_

#include "entity.h"
#include "SDL.h"

class Bullet : public Entity {

    public:
        Bullet(SDL_Renderer *sdl_renderer, float x, float y, int angle, int side);
        Bullet(SDL_Renderer *sdl_renderer);
        ~Bullet();
};

#endif 

