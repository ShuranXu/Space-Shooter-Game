#include "enemy.h"
#include "util.h"
#include "defs.h"
#include "powerup.h"

Enemy::Enemy(SDL_Renderer *sdl_renderer):Entity() {

    reload = 0;
    radius = 32;
    health = 5;
    side = SIDE_ENEMY;
    _sdl_renderer = sdl_renderer;
     image = Util::getParentDir() + "/gfx/enemy01.png";
    texture = Util::loadTexture(sdl_renderer, image);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    color.r = color.g = color.b = color.a = 255;
}

Enemy::Enemy(int x, int y, SDL_Renderer *sdl_renderer):Entity() {

    x = x;
    y = y;
    reload = 0;
    radius = 32;
    health = 5;
    side = SIDE_ENEMY;
    _sdl_renderer = sdl_renderer;
    image = Util::getParentDir() + "/gfx/enemy01.png";
    texture = Util::loadTexture(sdl_renderer, image);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    color.r = color.g = color.b = color.a = 255;
}

Enemy::~Enemy() {

    SDL_DestroyTexture(texture);
}

std::unique_ptr<Bullet> Enemy::tick(float player_x, float player_y){

	angle = Util::getAngle(x, y, player_x, player_x);
	Util::calcSlope(player_x, player_y, x, y, &dx, &dy);

    reload = MAX(reload - 1, 0);

    if (reload <= 0 && (Util::getDistance(x, y, player_x, player_y) < (SCREEN_HEIGHT >> 1))){
        reload = FPS * 3;
        return fireEnemyBullet(player_x, player_y);	
	}
    return nullptr;
}

std::unique_ptr<Bullet> Enemy::fireEnemyBullet(float player_x, float player_y){

	std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>(_sdl_renderer, x, y, angle, SIDE_ENEMY);
	
	bullet->color.r = 255;
	bullet->color.g = bullet->color.b = 0;
	bullet->color.a = 255;
	
	Util::calcSlope(player_x, player_y, bullet->x, bullet->y, &bullet->dx, &bullet->dy);
	
	bullet->dx *= 12;
	bullet->dy *= 12;

    return bullet;
}
