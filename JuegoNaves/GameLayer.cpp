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
	textPoints = new Text("0", WIDTH * 0.92, HEIGHT * 0.04, game);
	textPoints->content = to_string(points);

	points2 = 0;
	textPoints2 = new Text("0", WIDTH * 0.92, HEIGHT * 0.04, game);
	textPoints2->content = to_string(points2);

	player = new Player(50, 50, game);
	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	backgroundPoints = new Actor("res/icono_puntos.png",
		WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game);

	player2 = new Player("res/nave2.png", 50, 200, game);
	backgroundPoints2 = new Actor("res/icono_puntos.png",
		WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game);


	projectiles.clear(); // Vaciar por si reiniciamos el juego
	projectiles2.clear();

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
	if (controlShoot) {
		Projectile* newProjectile = player->shoot();
		if (newProjectile != NULL) {
			projectiles.push_back(newProjectile);
		}
	}
	if (controlShoot2) {
		Projectile* newProjectile2 = player2->shoot();
		if (newProjectile2 != NULL) {
			projectiles2.push_back(newProjectile2);
		}
	}

	//Jugador 1
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

	//Jugador 2
	// Eje X
	if (controlMoveX2 > 0) {
		player2->moveX(1);
	}
	else if (controlMoveX2 < 0) {
		player2->moveX(-1);
	}
	else {
		player2->moveX(0);
	}

	// Eje Y
	if (controlMoveY2 > 0) {
		player2->moveY(1);
	}
	else if (controlMoveY2 < 0) {
		player2->moveY(-1);
	}
	else {
		player2->moveY(0);
	}
}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
			
		//Jugador 1
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

		//Jugador 2
		case SDLK_RIGHT: // derecha
			controlMoveX2 = 1;
			break;
		case SDLK_LEFT: // izquierda
			controlMoveX2 = -1;
			break;
		case SDLK_UP: // arriba
			controlMoveY2 = -1;
			break;
		case SDLK_DOWN: // abajo
			controlMoveY2 = 1;
			break;
		case SDLK_p: // dispara
			controlShoot2 = true;
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
		case SDLK_RIGHT: // derecha
			if (controlMoveX2 == 1) {
				controlMoveX2 = 0;
			}
			break;
		case SDLK_LEFT: // izquierda
			if (controlMoveX2 == -1) {
				controlMoveX2 = 0;
			}
			break;
		case SDLK_UP: // arriba
			if (controlMoveY2 == -1) {
				controlMoveY2 = 0;
			}
			break;
		case SDLK_DOWN: // abajo
			if (controlMoveY2 == 1) {
				controlMoveY2 = 0;
			}
			break;
		case SDLK_p: // dispara
			controlShoot2 = false;
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

	player->update();
	player2->update();
	for (auto const& enemy : enemies) {
		enemy->update();
	}

	for (auto const& projectile : projectiles) {
		projectile->update();
	}

	for (auto const& projectile2 : projectiles2) {
		projectile2->update();
	}

	// Colisiones
	for (auto const& enemy : enemies) {
		if (player->isOverlap(enemy)) {
			init();
			return; // Cortar el for
		}
	}

	// Colisiones
	for (auto const& enemy : enemies) {
		if (player2->isOverlap(enemy)) {
			init();
			return; // Cortar el for
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

	for (auto const& projectile2 : projectiles2) {
		projectile2->draw();
	}

	player->draw();
	player2->draw();

	for (auto const& enemy : enemies) {
		enemy->draw();
	}

	textPoints->draw();
	backgroundPoints->draw();

	SDL_RenderPresent(game->renderer); // Renderiza
}