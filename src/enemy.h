#ifndef ENEMY_H_
#define ENEMY_H_

#include "entity.h"
#include <memory>
#include "SDL.h"
#include "bullet.h"

class Enemy : public Entity {

    public:
        Enemy(SDL_Renderer *sdl_renderer);
        Enemy(int x, int y, SDL_Renderer *sdl_renderer);
        ~Enemy();
        std::unique_ptr<Bullet> tick(float player_x, float player_y);

    private:
        std::unique_ptr<Bullet> fireEnemyBullet(float player_x, float player_y);
        SDL_Renderer *_sdl_renderer;
};

#endif


