#include "entity.h"


Entity::Entity(){

    health = 5;
	x = (SCREEN_WIDTH >> 1);
	y = (SCREEN_HEIGHT >> 1);
	dx = 0;
	dy = 0;
	angle = 0;
	w = 0;
	h = 0;
	reload = 0;
	image = "";
	side = 0;
	radius = 0;
	texture = nullptr;
}

Entity::Entity(float x, float y): x(x), y(y){

    health = 5;
	dx = 0;
	dy = 0;
	angle = 0;
	w = 0;
	h = 0;
	reload = 0;
	image = "";
	side = 0;
	radius = 0;
	texture = nullptr;
}

Entity::Entity(float x, float y, int angle, int side):x(x), y(y), angle(angle), side(side) {

	health = 5;
	dx = 0;
	dy = 0;
	w = 0;
	h = 0;
	reload = 0;
	image = "";
	radius = 0;
	texture = nullptr;
}