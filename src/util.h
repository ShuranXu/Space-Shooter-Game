#ifndef UTIL_H_
#define UTIL_H_

#include <string>
#include "SDL.h"
#include "entity.h"
#include "player.h"
#include "highscore.h"

class Util {

	public:
    	static float getAngle(int x1, int y1, int x2, int y2);
		static void calcSlope(int x1, int y1, int x2, int y2, float *dx, float *dy);
		static SDL_Texture* loadTexture(SDL_Renderer *sdl_renderer, std::string filename);
		static int getDistance(int x1, int y1, int x2, int y2);
		static int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
		static void tick(Entity *entity, Player *player);
		static bool highscoreComparator(const Highscore &a, const Highscore &b);
		static std::string getParentDir();
};


#endif 