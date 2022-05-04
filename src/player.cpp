#include "player.h"
#include "util.h"
#include "renderer.h"
#include "bullet.h"
#include <iostream>

Player::Player(SDL_Renderer *sdl_renderer): Entity() {

    reload = 0;
    score = 0;
	_sdl_renderer = sdl_renderer;
    side = SIDE_PLAYER;
    weaponType = WPN_PISTOL;

	for(int i=0;i<MAX_MOUSE_BUTTONS;++i)
		_mouse.button[i] = 0;
    
	image = Util::getParentDir() + "/gfx/donk.png";
    texture = Util::loadTexture(sdl_renderer, image);
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

}

Player::~Player() {

	std::cout << "Player Destructor called" << std::endl;
	SDL_DestroyTexture(texture);
}


void Player::keyDown(SDL_KeyboardEvent *event){

    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
		_keyboard[event->keysym.scancode] = 1;
	}
}

void Player::keyUp(SDL_KeyboardEvent *event){

    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
		_keyboard[event->keysym.scancode] = 0;
	}
}

void Player::doMouseButtonUp(SDL_MouseButtonEvent *event){

	_mouse.button[event->button] = 0;
}

void Player::doMouseButtonDown(SDL_MouseButtonEvent *event){

	_mouse.button[event->button] = 1;
}

void Player::resetInputText() {

	_input_text.clear();	
}

void Player::handleInput() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
  
      switch (e.type) {

		case SDL_QUIT:
			exit(0);
			break;

        case SDL_KEYUP:
          keyUp(&e.key);
          break;

        case SDL_KEYDOWN:
          	keyDown(&e.key);
          	break;

		case SDL_MOUSEBUTTONDOWN:
			doMouseButtonDown(&e.button);
			break;
			
		case SDL_MOUSEBUTTONUP:
			doMouseButtonUp(&e.button);
			break;
			
		case SDL_MOUSEWHEEL:
			_mouse.wheel = e.wheel.y;
			break;

		case SDL_TEXTINPUT:
			char input[2] = {0,0};
			std::string(e.text.text).copy(input,1,0);
			input[1] = '\0';
			_input_text = std::string(input);
			break;
      }
    }

  // Update mouse's position
  SDL_GetMouseState(&_mouse.x, &_mouse.y);
}

void Player::updatePlayerPosition(){

	dx *= 0.85;
	dy *= 0.85;
	
	if (_keyboard[SDL_SCANCODE_W])
	{
		dy = -PLAYER_SPEED;
	}
	
	if (_keyboard[SDL_SCANCODE_S])
	{
		dy = PLAYER_SPEED;
	}
	
	if (_keyboard[SDL_SCANCODE_A])
	{
		dx = -PLAYER_SPEED;
	}
	
	if (_keyboard[SDL_SCANCODE_D])
	{
		dx = PLAYER_SPEED;
	}
	
	angle = Util::getAngle(x, y, _mouse.x, _mouse.y);
}

std::unique_ptr<Bullet> Player::fireDonkBullet() {

	switch (weaponType)
	{
		case WPN_UZI:
			return fireDonkUzi();
			
		default:
			return fireDonkPistol();
	}
}

std::unique_ptr<Bullet> Player::updatePlayerAmmo(int *ammo) {

	if (reload == 0 && ammo[weaponType] > 0 && _mouse.button[SDL_BUTTON_LEFT]){
		ammo[weaponType]--;
        return fireDonkBullet();
	}
	
	if (_mouse.wheel < 0){
		weaponType = WPN_PISTOL;
		_mouse.wheel = 0;
	}
	
	if (_mouse.wheel > 0){
		weaponType = WPN_UZI;
		_mouse.wheel = 0;
	}
	
	if (_mouse.button[SDL_BUTTON_RIGHT]){
		if (weaponType == WPN_PISTOL && ammo[WPN_PISTOL] == 0){

			ammo[WPN_PISTOL] = 20;
		}
	    _mouse.button[SDL_BUTTON_RIGHT] = 0;
	}

    return nullptr;
}

std::unique_ptr<Bullet> Player::fireDonkUzi(){

	std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>(_sdl_renderer, x, y, angle, SIDE_PLAYER);
	Util::calcSlope(_mouse.x, _mouse.y, bullet->x, bullet->y, &bullet->dx, &bullet->dy);
	
	bullet->dx *= 16;
	bullet->dy *= 16;
	//set player's load to 4
	reload = 4;

    return bullet;
}

std::unique_ptr<Bullet> Player::fireDonkPistol() {

    std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>(_sdl_renderer, x, y, angle, SIDE_PLAYER);
	Util::calcSlope(_mouse.x, _mouse.y, bullet->x, bullet->y, &bullet->dx, &bullet->dy);
	
	bullet->dx *= 16;
	bullet->dy *= 16;
	//set player's load to 16
	reload = 16;

    return bullet;
}

void Player::draw() {
	Renderer::blitRotated(_sdl_renderer, texture, x, y, angle);
}