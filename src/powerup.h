
#ifndef POWERUP_H_
#define POWERUP_H_

#include "entity.h"
#include <memory>
#include "SDL.h"
#include "player.h"

class Powerup : public Entity {

    public:
        Powerup();
        Powerup(int x, int y);
        ~Powerup();
        void tick();
        virtual void touch(Player *player) = 0;
};

class UziPowerup : public Powerup {

    public:
        UziPowerup(SDL_Renderer *sdl_renderer, int *ammo);
        UziPowerup(int x, int y, SDL_Renderer *sdl_renderer, int *ammo);
        ~UziPowerup();
        void touch(Player *player);
    private:
        int *_ammo;
  
};

class HealthPowerup : public Powerup {

    public:
        HealthPowerup(SDL_Renderer *sdl_renderer);
        HealthPowerup(int x, int y, SDL_Renderer *sdl_renderer);
        ~HealthPowerup();
        void touch(Player *player);
};

#endif 