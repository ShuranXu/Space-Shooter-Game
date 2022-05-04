#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <string>
#include <memory>
#include "defs.h"
#include "SDL.h"
#include "bullet.h"
#include "util.h"

class Renderer {
 public:
    Renderer(SDL_Window *sdl_window, SDL_Renderer *sdl_renderer);
    ~Renderer();

    void presentScene();
    void prepareScene();
    void drawGameScreen(Mouse mouse, int *ammo, int health, int score, int weapon_type);
    static void blit(SDL_Renderer *sdl_renderer, SDL_Texture *texture, int x, int y, int center);
    static void blitRotated(SDL_Renderer *sdl_renderer, SDL_Texture *texture, int x, int y, float angle);
    static void blitRect(SDL_Renderer *sdl_renderer, SDL_Texture *texture, SDL_Rect *src, int x, int y);
    static void drawText(SDL_Renderer *sdl_renderer, int x, int y, int r, int g, int b, int align, char *format, ...);
   
 private:
    SDL_Window *_sdl_window;
    SDL_Renderer *_sdl_renderer;
    SDL_Texture *_targetterTexture;
    SDL_Texture *_gridTexture;
    SDL_Texture *_fontTexture;
    int *_ammo;

    void drawGrid();
    void drawHud(int health, int score, int weapon_type);
    void drawWeaponInfo(char *name, int type, int weapon_type, int x, int y);
    
};

#endif