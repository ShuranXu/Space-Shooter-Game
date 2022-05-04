#ifndef PLAYER_H_
#define PLAYER_H_

#include "entity.h"
#include "bullet.h"
#include <memory>
#include "SDL.h"

class Player : public Entity {

    public:
        Player(SDL_Renderer *sdl_renderer);
        ~Player();
	    int weaponType;
        int score;
        void updatePlayerPosition();
        std::unique_ptr<Bullet> updatePlayerAmmo(int *ammo);
        void handleInput();
        void draw();
        int *getKeyboard() {return _keyboard;}
        Mouse getMouse() {return _mouse;};
        std::string getUserInput() { return _input_text; }
        void resetInputText();

    private:
        void keyDown(SDL_KeyboardEvent *event);
        void keyUp(SDL_KeyboardEvent *event);
        void doMouseButtonUp(SDL_MouseButtonEvent *event);
        void doMouseButtonDown(SDL_MouseButtonEvent *event);
        std::unique_ptr<Bullet> fireDonkBullet();
        std::unique_ptr<Bullet> fireDonkPistol();
        std::unique_ptr<Bullet> fireDonkUzi();
    
        int _keyboard[MAX_KEYBOARD_KEYS] = {0};
        SDL_Renderer *_sdl_renderer;
        Mouse _mouse;
        std::string _input_text;
};

#endif 
