#include "SpaceShip.h"

#include <pthread.h>

#include "Bullet.h"

SpaceShip::SpaceShip(Game* game, int player_x, int player_y, int player_dir) {
	this->game = game;
	this->player_x = player_x;
	this->player_y = player_y;
	this->player_dir = player_dir;
	this->dead = false;
}

SpaceShip::~SpaceShip() {
	// TODO Auto-generated destructor stub
}

Game* SpaceShip::getGame() {
	return this->game;
}

int SpaceShip::getX() {
	return this->player_x;
}

void SpaceShip::setX(int x) {
	this->player_x = x;
}

void SpaceShip::moveX(int x) {
	this->player_x += x;
}

int SpaceShip::getY() {
	return this->player_y;
}

void SpaceShip::setY(int y) {
	this->player_y = y;
}

void SpaceShip::moveY(int y) {
	this->player_y += y;
}

void SpaceShip::setDead(bool dead) {
	this->dead = dead;
}

bool SpaceShip::isDead() {
	return this->dead;
}

void SpaceShip::fireBullet() {
	Bullet* bullet = new Bullet(this, player_x, player_y, player_dir);

	pthread_t bulletThread;
	pthread_create(&bulletThread, NULL, &Bullet::bulletExecution, bullet);
}
