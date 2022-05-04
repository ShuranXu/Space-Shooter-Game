

#include "title.h"
#include "defs.h"
#include "util.h"
#include "renderer.h"

Title::Title(SDL_Renderer *sdl_renderer) {

	std::string parent_path = Util::getParentDir();
    _battle_arena_texture = Util::loadTexture(sdl_renderer, parent_path + "/gfx/battleArena.png");
	_donk_texture[0] = Util::loadTexture(sdl_renderer, parent_path + "/gfx/D.png");
	_donk_texture[1] = Util::loadTexture(sdl_renderer, parent_path + "/gfx/O.png");
	_donk_texture[2] = Util::loadTexture(sdl_renderer, parent_path + "/gfx/N.png");
	_donk_texture[3] = Util::loadTexture(sdl_renderer, parent_path + "/gfx/K.png");
	_donk_texture[4] = Util::loadTexture(sdl_renderer, parent_path + "//gfx/!.png");

    for(int i=0; i < 5; ++i)
        _donk_angle[i] = rand() % 360;
	
	if (rand() % 5 == 0){
		_battle_arena_Y = -200;
		_battle_arena_dy = 0;
	}
	
	_timeout = FPS * 5;
    _sdl_renderer = sdl_renderer;
}

Title::~Title() {

	std::cout << "Title Destructor called" << std::endl;
	SDL_DestroyTexture(_battle_arena_texture);
	for(int i = 0; i < 5; i++)
		SDL_DestroyTexture(_donk_texture[i]);
}

void Title::tuneTitle() {

    _battle_arena_dy = MIN(_battle_arena_dy + 0.25, ARENA_DY_LIMIT);
    _battle_arena_Y = MIN(_battle_arena_Y + _battle_arena_dy, ARENA_Y_LIMIT);
    
    if (_battle_arena_Y == ARENA_Y_LIMIT){
        _battle_arena_dy = -_battle_arena_dy * 0.5;
        
        if (_battle_arena_dy > -1)
            _battle_arena_dy = 0; 
    }
    
    for (int i = 0 ; i < 5 ; i++)
        _donk_angle[i] = MIN(_donk_angle[i] + 2, DONK_ANGLE_LIMIT);

    _timeout--;
}

void Title::displayTitle() {

    drawLogo();
	
	if (_timeout % 40 < 20){
		Renderer::drawText(_sdl_renderer,SCREEN_WIDTH / 2, 600, 255, 255, 255, TEXT_CENTER, (char *)"PRESS FIRE TO PLAY!");
    }
}

void Title::drawLogo(){

	int x = -300;
	Renderer::blit(_sdl_renderer, _battle_arena_texture, SCREEN_WIDTH / 2, _battle_arena_Y, 1);
	
	for (int i = 0 ; i < 5 ; i++){
		Renderer::blitRotated(_sdl_renderer, _donk_texture[i], (SCREEN_WIDTH / 2) + x, 350, _donk_angle[i]);
		x += 150;
	}
}

