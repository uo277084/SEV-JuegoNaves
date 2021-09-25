#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Text.h"
#include "Audio.h" 


#include <list>

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;
	void keysToControls(SDL_Event event);
	int newEnemyTime = 0;

	Player* player;
	Player* player2;
	Background* background;
	Actor* backgroundPoints;
	Actor* backgroundPoints2;
	Audio* audioBackground;
	Text* textPoints;
	Text* textPoints2;
	int points;
	int points2;
	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;
	bool controlShoot2 = false;
	int controlMoveY2 = 0;
	int controlMoveX2 = 0;
	list<Enemy*> enemies;
	list<Projectile*> projectiles;
	list<Projectile*> projectiles2;
};