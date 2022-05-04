#include "util.h"
#include "defs.h"
#include <cmath>
#include "SDL2/SDL_image.h"
#include <iostream>
#include <filesystem>

float Util::getAngle(int x1, int y1, int x2, int y2){
	    float angle = -90 + std::atan2(y1 - y2, x1 - x2) * (180 / PI);
	    return angle >= 0 ? angle : 360 + angle;
}

void Util::calcSlope(int x1, int y1, int x2, int y2, float *dx, float *dy) {

	int steps = MAX(abs(x1 - x2), abs(y1 - y2));

	if (steps == 0){
		*dx = *dy = 0;
		return;
	}

	*dx = (x1 - x2);
	*dx /= steps;

	*dy = (y1 - y2);
	*dy /= steps;
}

SDL_Texture* Util::loadTexture(SDL_Renderer *sdl_renderer, std::string filename) {

	if(! sdl_renderer)
        std::cout << "loadTexture(): sdl_renderer is NULL !" << std::endl;

	SDL_Texture *texture = nullptr;
	// SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s ...", filename.c_str());
    texture = IMG_LoadTexture(sdl_renderer, filename.c_str());
    if(! texture)
        std::cout << "loadTexture(): IMG_LoadTexture() " << filename << " returns NULL !" << std::endl;

    return texture;
}

int Util::collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2){

	return (MAX(x1, x2) < MIN(x1 + w1, x2 + w2)) && (MAX(y1, y2) < MIN(y1 + h1, y2 + h2));
}

int Util::getDistance(int x1, int y1, int x2, int y2){
	
	int x, y;

	x = x2 - x1;
	y = y2 - y1;

	return sqrt(x * x + y *y);
}

void Util::tick(Entity *entity, Player *player){

	entity->angle = Util::getAngle(entity->x, entity->y, player->x, player->y);
	Util::calcSlope(player->x, player->y, entity->x, entity->y, &entity->dx, &entity->dy);
}

bool Util::highscoreComparator(const Highscore &a, const Highscore &b){
	return (a.score - b.score) > 0;
}

std::string Util::getParentDir() {

	std::string current_path = std::filesystem::absolute(std::filesystem::path("."));
	std:: string current_dir = current_path.substr(0, current_path.find_last_of("/"));
	std:: string parent_dir = current_dir.substr(0, current_dir.find_last_of("/"));
	return parent_dir;
}