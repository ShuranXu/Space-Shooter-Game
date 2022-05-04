#ifndef ENTITY_H_
#define ENTITY_H_

#include "defs.h"
#include "SDL.h"
#include <iostream>



class Entity {

    public:
        Entity();
        Entity(float x, float y);
        Entity(float x, float y, int angle, int side);
        float x;
        float y;
        int w;
        int h;
        float dx;
        float dy;
        int health;
        int angle;
        int reload;
        int side;
        int radius;
        SDL_Color color;
        std::string image;
        SDL_Texture *texture;
};


#endif 