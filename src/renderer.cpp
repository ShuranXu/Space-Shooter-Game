#include "renderer.h"
#include "SDL2/SDL_image.h"
#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <memory>

Renderer::Renderer(SDL_Window *sdl_window, SDL_Renderer *sdl_renderer): _sdl_renderer(sdl_renderer),
 _sdl_window(sdl_window) {

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    SDL_ShowCursor(SDL_DISABLE);

	std::string parent_path = Util::getParentDir();
	_targetterTexture = Util::loadTexture(_sdl_renderer, parent_path + "/gfx/targetter.png");
	_gridTexture = Util::loadTexture(_sdl_renderer, parent_path + "/gfx/grid.png");
	_fontTexture = Util::loadTexture(sdl_renderer, parent_path + "/gfx/font.png");
}

Renderer::~Renderer() {

	std::cout << "Renderer Destructor called" << std::endl;
	//destroy textures
	SDL_DestroyTexture(_fontTexture);
	SDL_DestroyTexture(_gridTexture);
	SDL_DestroyTexture(_targetterTexture);
}

void Renderer::prepareScene() {

    SDL_SetRenderDrawColor(_sdl_renderer, 32, 32, 32, 255);
	SDL_RenderClear(_sdl_renderer);
}

void Renderer::presentScene() {

    SDL_RenderPresent(_sdl_renderer);
}

void Renderer::drawGameScreen(Mouse mouse, int *ammo, int health, int score, int weapon_type) {

    _ammo = ammo;
    drawGrid();
	drawHud(health, score, weapon_type);
	blit(_sdl_renderer,_targetterTexture, mouse.x, mouse.y, 1);
}

void Renderer::drawHud(int health, int score, int weapon_type) {

    drawText(_sdl_renderer,100, 10, 255, 255, 255, TEXT_LEFT, (char *)"HEALTH:%d", health);
	drawText(_sdl_renderer, 400, 10, 255, 255, 255, TEXT_LEFT, (char *)"SCORE:%05d", score);
	drawWeaponInfo((char *)"PISTOL", WPN_PISTOL, weapon_type, 700, 10);
	drawWeaponInfo((char *)"UZI", WPN_UZI, weapon_type, 1000, 10);
}

void Renderer::blit(SDL_Renderer *sdl_renderer, SDL_Texture *texture, int x, int y, int center) {

	SDL_Rect dest;
	
	dest.x = x;
	dest.y = y;

	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	
	if (center){
		dest.x -= dest.w / 2;
		dest.y -= dest.h / 2;
	}
	SDL_RenderCopy(sdl_renderer, texture, NULL, &dest);
}

void Renderer::blitRect(SDL_Renderer *sdl_renderer, SDL_Texture *texture, SDL_Rect *src, int x, int y) {

	SDL_Rect dest;
	
	dest.x = x;
	dest.y = y;
	dest.w = src->w;
	dest.h = src->h;
    SDL_RenderCopy(sdl_renderer, texture, src, &dest);
}

void Renderer::blitRotated(SDL_Renderer *sdl_renderer, SDL_Texture *texture, int x, int y, float angle){

	SDL_Rect dstRect;
	
	dstRect.x = x;
	dstRect.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dstRect.w, &dstRect.h);
	dstRect.x -= (dstRect.w / 2);
	dstRect.y -= (dstRect.h / 2);
    SDL_RenderCopyEx(sdl_renderer, texture, NULL, &dstRect, angle, NULL, SDL_FLIP_NONE);
}

void Renderer::drawText(SDL_Renderer *sdl_renderer, int x, int y, int r, int g, int b, int align, char *format, ...){
    
	int i, len, c;
	SDL_Rect rect;
	va_list args;
	char drawTextBuffer[MAX_LINE_LENGTH] = {0};
	std::string font_image_path = Util::getParentDir() + "/gfx/font.png";
	SDL_Texture *fontTexture = Util::loadTexture(sdl_renderer, font_image_path);

	va_start(args, format);
	vsprintf(drawTextBuffer, format, args);
	va_end(args);
	
	len = strlen(drawTextBuffer);
	
	switch (align){

		case TEXT_RIGHT:
			x -= (len * GLYPH_WIDTH);
			break;
			
		case TEXT_CENTER:
			x -= (len * GLYPH_WIDTH) / 2;
			break;
	}
	
	rect.w = GLYPH_WIDTH;
	rect.h = GLYPH_HEIGHT;
	rect.y = 0;
	
	SDL_SetTextureColorMod(fontTexture, r, g, b);
	
	for (i = 0 ; i < len ; i++)
	{
		c = drawTextBuffer[i];
		
		if (c >= ' ' && c <= 'Z')
		{
			rect.x = (c - ' ') * GLYPH_WIDTH;
			
			blitRect(sdl_renderer,fontTexture, &rect, x, y);
			
			x += GLYPH_WIDTH;
		}
	}
}

void Renderer::drawWeaponInfo(char *name, int type, int weapon_type, int x, int y){
	
	int r, g, b;
	
	if(weapon_type == WPN_PISTOL && name == "PISTOL"){
		r = b = 0;
		g = 255;
	}
	else if(weapon_type == WPN_UZI && name == "UZI"){
		r = b = 0;
		g = 255;
	}
	else{
		r = g = b = 255;
	}
	
	drawText(_sdl_renderer, x, y, r, g, b, TEXT_LEFT, (char *)"%s:%03d", name, _ammo[type]);
}

void Renderer::drawGrid(){
	
	for (int y = 0 ; y < SCREEN_HEIGHT ; y += GRID_SIZE){
		for (int x = 0 ; x < SCREEN_WIDTH ; x += GRID_SIZE){
			blit(_sdl_renderer, _gridTexture, x, y, 0);
		}
	}
}
