#ifndef DEFS_H_
#define DEFS_H_

#include "SDL.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define FPS 					60
#define SCREEN_WIDTH   			1280
#define SCREEN_HEIGHT  			720
#define MAX_LINE_LENGTH        	1024
#define MAX_KEYBOARD_KEYS 		350
#define MAX_SND_CHANNELS 		16
#define PLAYER_SPEED 			6
#define MAX_MOUSE_BUTTONS		6
#define PI 						3.14159265358979323846
#define GLYPH_WIDTH  			18
#define GLYPH_HEIGHT 			29
#define GRID_SIZE 				64
#define BULLET_HIT_RANGE 		16
#define DEATH_EFFECT_RANGE 		64
#define DONK_CHARACTER_NUM 		5	
#define ARENA_Y_LIMIT       	200
#define ARENA_DY_LIMIT      	25
#define DONK_ANGLE_LIMIT 		360
#define NUM_HIGHSCORES 			8


typedef struct {
	int x;
	int y;
	int button[MAX_MOUSE_BUTTONS];
	int wheel;
} Mouse;


enum
{
	TEXT_LEFT,
	TEXT_CENTER,
	TEXT_RIGHT
};

// enum for weapon type
enum
{
	WPN_PISTOL,
	WPN_UZI,
	WPN_MAX
};

enum
{
	SIDE_NONE,
	SIDE_PLAYER,
	SIDE_ENEMY
};

#endif 