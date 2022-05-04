#include "effect.h"
#include "defs.h"
#include "util.h"

Effect::Effect() {

    x = 0;
    y = 0;
    dx = 400 - rand() % 800;
	dy = 400 - rand() % 800;	
	dx /= 100;
	dy /= 100;

    color.r = 255;
    color.g = 128 + rand() % 128;
    color.b = 0;
    color.a = rand() % 255;
    
    life = rand() % FPS;
    texture = nullptr;
}

Effect::Effect(SDL_Renderer *sdl_renderer, float x, float y, int side):x(x),y(y) {

    dx = 400 - rand() % 800;
	dy = 400 - rand() % 800;	
	dx /= 100;
	dy /= 100;

    color.r = 255;
    color.g = 128 + rand() % 128;
    color.b = 0;
    color.a = rand() % 255;
    
    life = rand() % FPS;

    if(side == SIDE_ENEMY)
        texture = Util::loadTexture(sdl_renderer,Util::getParentDir() + "/gfx/whiteSquare16.png");
    else
        texture = Util::loadTexture(sdl_renderer,Util::getParentDir() + "/gfx/whiteSquare8.png");
}

Effect::~Effect() {

    if(texture)
        SDL_DestroyTexture(texture);
}