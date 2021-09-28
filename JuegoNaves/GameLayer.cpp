#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
	//llama al constructor del padre : Layer(renderer)
	init();
}

void GameLayer::init() {
	audioBackground = new Audio("res/musica_ambiente.mp3", true);
	audioBackground->play();

	points = 0;
	textPoints = new Text("0", WIDTH * 0.92, HEIGHT * 0.05, game);
	textPoints->content = to_string(points);

	player = new Player(50, 50, game);
	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	backgroundPoints = new Actor("res/icono_puntos.png",
		WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game);

	textShootsLeft = new Text("0", WIDTH * 0.75, HEIGHT * 0.05, game);
	textShootsLeft->content = to_string(disparos);
	backgroundShoots = new Actor("res/disparo_enemigo.png",
		WIDTH * 0.68, HEIGHT * 0.05, 24, 24, game);
	powerups.clear();

	projectiles.clear(); // Vaciar por si reiniciamos el juego

	enemies.clear(); // Vaciar por si reiniciamos el juego
	enemies.push_back(new Enemy(300, 50, game));
	enemies.push_back(new Enemy(300, 200, game));
}

void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		keysToControls(event);
	}
	//procesar controles
	// Disparar
	if (controlShoot && disparos > 0) {
		Projectile* newProjectile = player->shoot();
		if (newProjectile != NULL) {
			projectiles.push_back(newProjectile);
			disparos--;
			textShootsLeft->content = to_string(disparos);
		}
	}

	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}

	// Eje Y
	if (controlMoveY > 0) {
		player->moveY(1);
	}
	else if (controlMoveY < 0) {
		player->moveY(-1);
	}
	else {
		player->moveY(0);
	}
}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
			break;
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_SPACE: // dispara
			controlShoot = true;
			break;
		}
	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlShoot = false;
			break;
		}
	}
}

void GameLayer::update() {
	background->update();

	// Generar enemigos
	newEnemyTime--;
	if (newEnemyTime <= 0) {
		int rX = (rand() % (600 - 500)) + 1 + 500;
		int rY = (rand() % (300 - 60)) + 1 + 60;
		enemies.push_back(new Enemy(rX, rY, game));
		newEnemyTime = 110;
	}

	//Generar recarga de disparo
	newPowerUpTime--;
	if (newPowerUpTime <= 0) {
		int rx = (rand() % 300) + 100;
		int ry = (rand() % (300 - 60)) + 1 + 60;
		powerups.push_back(new PowerUp(rx, ry, game));
		newPowerUpTime = 200;
	}

	player->update();
	for (auto const& enemy : enemies) {
		enemy->update();
	}

	for (auto const& projectile : projectiles) {
		projectile->update();
	}

	// Colisiones
	for (auto const& enemy : enemies) {
		if (player->isOverlap(enemy)) {
			init();
			return; // Cortar el for
		}
	}

	//Colisión con recarga de disparos

	list<PowerUp*> deletePowerUps;

	for (auto const& powerup : powerups) {
		if (player->isOverlap(powerup)) {
			bool eInList = std::find(deletePowerUps.begin(),
				deletePowerUps.end(),
				powerup) != deletePowerUps.end();

			if (!eInList) {
				deletePowerUps.push_back(powerup);
			}

			disparos += 10;
			textShootsLeft->content = to_string(disparos);
		}
	}

	// Colisiones , Enemy - Projectile

	list<Enemy*> deleteEnemies;
	list<Projectile*> deleteProjectiles;
	
	for (auto const& enemy : enemies) {
		for (auto const& projectile : projectiles) {
			if (enemy->isOverlap(projectile)) {
				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end();

				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}

				bool eInList = std::find(deleteEnemies.begin(),
					deleteEnemies.end(),
					enemy) != deleteEnemies.end();

				if (!eInList) {
					deleteEnemies.push_back(enemy);
				}
				points++;
				textPoints->content = to_string(points);
			}
		}
	}

	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
	}
	deleteEnemies.clear();

	for (auto const& delPowerUp : deletePowerUps) {
		powerups.remove(delPowerUp);
	}
	deletePowerUps.clear();

	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
	}
	deleteProjectiles.clear();

	cout << "update GameLayer" << endl;
	
	for (auto const& projectile : projectiles) {
		if (projectile->isInRender() == false) {

			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();

			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
		}
	}
	

}

void GameLayer::draw() {
	background->draw();

	for (auto const& projectile : projectiles) {
		projectile->draw();
	}

	player->draw();

	for (auto const& enemy : enemies) {
		enemy->draw();
	}

	for (auto const& powerup : powerups) {
		powerup->draw();
	}

	textPoints->draw();
	backgroundPoints->draw();

	textShootsLeft->draw();
	backgroundShoots->draw();

	SDL_RenderPresent(game->renderer); // Renderiza
}