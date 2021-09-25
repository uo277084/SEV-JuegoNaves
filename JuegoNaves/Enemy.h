#pragma once

#include "Actor.h"
#include "Animation.h"

class Enemy : public Actor
{
public:
	Enemy(float x, float y, Game* game);
	void draw() override; // va a sobrescribir
	void update();
	Animation* aMoving;
	Animation* animation; //referencia a la animación mostrada
};