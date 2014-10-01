#include "Game.h"

#include <iostream>
#include <stdio.h>
#include <conio.h>

#include <pthread.h>

#include "../Entities/Player.h"
#include "../Entities/Enemy.h"

Game::Game() {
	this->writeMutex = PTHREAD_MUTEX_INITIALIZER;

	this->lifes = 5;
	this->points = 0;

	player = new Player(this, 40, 23);

	updatePoints();
	updateLifes();

	pthread_t enemiesThread;
	pthread_create(&enemiesThread, NULL, &Game::enemiesControl, this);
}

Game::~Game() {
	pthread_mutex_destroy(&writeMutex);
}

Player* Game::getPlayer() {
	return this->player;
}

Enemy** Game::getEnemies() {
	return this->enemies;
}

unsigned int Game::getEnemiesCount() {
	return sizeof(enemies) / sizeof(*enemies);
}

unsigned int Game::getLifes() {
	return this->lifes;
}

void Game::setLifes(int lifes) {
	this->lifes = lifes;
}

void Game::changeLifes(int lifes) {
	this->lifes += lifes;
}

int Game::getPoints() {
	return this->points;
}

void Game::setPoints(int points) {
	this->points = points;
}

void Game::changePoints(int points) {
	this->points += points;
}

void Game::writeAtPosition(unsigned int x, unsigned int y, const char* text) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;

	pthread_mutex_lock(&writeMutex);
	SetConsoleCursorPosition(hConsole, pos);
	std::cout << text;
	pthread_mutex_unlock(&writeMutex);
}

void Game::erasePosition(unsigned int x, unsigned int y) {
	writeAtPosition(x, y, " ");
}

void Game::updatePoints() {
	char pointsText[15];
	sprintf(pointsText, "Points: %5i", points);
	writeAtPosition(0, 24, pointsText);
}

void Game::updateLifes() {
	char lifesText[10];
	sprintf(lifesText, "Lifes: %i", lifes);
	writeAtPosition(20, 24, lifesText);
}

void* Game::enemiesControl(void* context) {
	Game* c = (Game*) context;

    for (int i = 0; i < ENEMIES_MAX; i++) {
    	c->enemies[i] = new Enemy(c, rand()%X_MAX, rand()%5);
    	c-> writeAtPosition(c->enemies[i]->getX(), c->enemies[i]->getY(), "T");
    }

    do {
        for (int i = 0; i < ENEMIES_MAX; i++) {
            if (!c->enemies[i]->isDead()) {
            	c->erasePosition(c->enemies[i]->getX(), c->enemies[i]->getY());
            	c->enemies[i]->moveX(rand()%3-1);
                if (c->enemies[i]->getX() < 0) {
                	c->enemies[i]->setX(0);
                }
                if (c->enemies[i]->getX() >= X_MAX) {
                	c->enemies[i]->setX(X_MAX-1);
                }
                c->enemies[i]->moveY(rand()%3-1);
                if (c->enemies[i]->getY() < 0) {
                	c->enemies[i]->setY(0);
                }
                if (c->enemies[i]->getY() >= 5) {
                	c->enemies[i]->setY(4);
                }
                c->writeAtPosition(c->enemies[i]->getX(), c->enemies[i]->getY(), "T");
                c->enemies[i]->fireBullet();
                Sleep(250);
            }
        }
    } while(true);

    pthread_exit(NULL);
    return 0;
}
