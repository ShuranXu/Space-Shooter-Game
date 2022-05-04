#ifndef _TITLE_H_
#define _TITLE_H_


#include "SDL.h"

class Title {

    public:
        Title(SDL_Renderer *sdl_renderer);
        ~Title();
        void tuneTitle();
        void displayTitle();
    private:
        SDL_Renderer *_sdl_renderer;
        SDL_Texture *_battle_arena_texture;
        SDL_Texture *_donk_texture[5];
        float _donk_angle[5];
        float _battle_arena_dy;
        int _timeout;
        int _battle_arena_Y;
        void drawLogo();
};

#endif 