#include "bullet.h"
#include "defs.h"
#include "util.h"

Bullet::Bullet(SDL_Renderer *sdl_renderer, float x, float y, int angle, int side): 
Entity( x, y, angle, side) {

    reload = 0;
    radius = 16;
    image = Util::getParentDir() + "/gfx/donkBullet.png";
    texture = Util::loadTexture(sdl_renderer, image);
    health = FPS * 2;
    color.r = 128;
	color.g = color.b = color.a = 255;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
}

Bullet::Bullet(SDL_Renderer *sdl_renderer):Entity() {

    reload = 0;
    image = Util::getParentDir() + "/gfx/donkBullet.png";
    texture = Util::loadTexture(sdl_renderer, image);
    health = FPS * 2;
    side = SIDE_PLAYER;
}

Bullet::~Bullet() {

    SDL_DestroyTexture(texture);
}