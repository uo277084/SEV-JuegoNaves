#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/jugador.png", x, y, 50, 57, game) {

	audioShoot = new Audio("res/efecto_disparo.wav", false);
	// lo que mide la imagen
	fileWidth = 50;
	//Vidas iniciales del jugador
	live = 3;
}

void Player::update() {
	if (shootTime > 0) {
		shootTime--;
	}

	if (x <= 0 || x >= WIDTH) {
		moveX(0);
		if (x <= 0) {
			x++;
		}
		else {
			x--;
		}
	}
	if (y <= 0 || y >= HEIGHT) {
		moveY(0);
		if (y <= 0) {
			y++;
		}
		else {
			y--;
		}
	}

	x = x + vx;
	y = y + vy;
}

void Player::moveX(float axis) {
	vx = axis * 3;
}

void Player::moveY(float axis) {
	vy = axis * 3;
}

Projectile* Player::shoot() {
	if (shootTime == 0) {
		audioShoot->play();
		shootTime = shootCadence;
		return new Projectile(x, y, game);
	}
	else {
		return NULL;
	}
}

void Player::shooted() {
	live--;
}

int Player::getLive() {
	return live;
}