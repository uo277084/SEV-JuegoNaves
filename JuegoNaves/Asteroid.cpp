#include "Asteroid.h"

Asteroid::Asteroid(float x, float y, Game* game)
	: Actor("res/asteroide.png", x, y, 36, 40, game) {

	vx = 1;
	vy = 2;
}

void Asteroid::update() {
	vx = -1;
	x = x + vx;
	if (y <= 0 || y >= HEIGHT) {
		vy *= -1;
	}
	y = y + vy;
}