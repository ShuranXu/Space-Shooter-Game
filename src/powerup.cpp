#include "powerup.h"
#include "util.h"

/*******************************************************************
 * 
 *  Powerup Class Implementation
 * 
 * *******************************************************************/
Powerup::Powerup():Entity(){

    x = 0;
	y = 0;
	health = FPS * 10;
	radius = 16;
   
	dx = -200 + (rand() % 400);
	dy = -200 + (rand() % 400);
	
	dx /= 100;
	dy /= 100;
}

Powerup::Powerup(int x, int y):
Entity(x,y){

	health = FPS * 5;
	radius = 16;
    
	dx = -200 + (rand() % 400);
	dy = -200 + (rand() % 400);
	
	dx /= 100;
	dy /= 100;
}

 Powerup::~Powerup() {

 }

void Powerup::tick() {

    health--;
	dx *= 0.98;
	dy *= 0.98;
}

/*******************************************************************
 * 
 *  UziPowerup Class Implementation
 * 
 * *******************************************************************/
UziPowerup::UziPowerup(SDL_Renderer *sdl_renderer, int *ammo):
Powerup() {

    _ammo = ammo;
	image = Util::getParentDir() + "/gfx/uzi.png";
    texture = Util::loadTexture(sdl_renderer, image);
}

UziPowerup::UziPowerup(int x, int y, SDL_Renderer *sdl_renderer, int *ammo):
Powerup(x,y) {

    _ammo = ammo;
	image = Util::getParentDir() + "/gfx/uzi.png";
    texture = Util::loadTexture(sdl_renderer, image);
}

 UziPowerup::~UziPowerup() {

	SDL_DestroyTexture(texture);
 }

void UziPowerup::touch(Player *player){

	health = 0;
	_ammo[WPN_UZI] += 25;
}

/*******************************************************************
 * 
 *  HealthPowerup Class Implementation
 * 
 * *******************************************************************/
HealthPowerup::HealthPowerup(SDL_Renderer *sdl_renderer):
Powerup() {

	image = Util::getParentDir() + "/gfx/health.png";
    texture = Util::loadTexture(sdl_renderer, image);
}

HealthPowerup::HealthPowerup(int x, int y, SDL_Renderer *sdl_renderer):
Powerup(x,y) {

	image = Util::getParentDir() + "/gfx/health.png";
    texture = Util::loadTexture(sdl_renderer, image);
}

 HealthPowerup::~HealthPowerup() {

	SDL_DestroyTexture(texture);
 }

void HealthPowerup::touch(Player *player){

	health = 0;
	player->health++;
}