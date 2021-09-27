#pragma once
#include "Actor.h"

class Asteroid : public Actor
{
public:
	Asteroid(float x, float y, Game* game);
	void update();
};