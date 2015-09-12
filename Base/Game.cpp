#include "Game.h"

#ifdef _WIN32
#include <conio.h>
#else
#include <unistd.h>
#include <curses.h>
#endif

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include "../Entities/Player.h"
#include "../Entities/Enemy.h"

Game::Game(unsigned int max_x, unsigned int max_y) {
	this->writeMutex = PTHREAD_MUTEX_INITIALIZER;

	this->max_x = max_x;
	this->max_y = max_y;
	this->maxEnemies = this->max_x * 5 / 10;

	this->lifes = 5;
	this->points = 0;

	player = new Player(this, this->max_x / 2, this->max_y - 2);

	updatePoints();
	updateLifes();

	pthread_t enemiesThread;
	pthread_create(&enemiesThread, NULL, &Game::enemiesControl, this);
}

Game::~Game() {
	pthread_mutex_destroy(&writeMutex);
}

unsigned int Game::getMaxX() {
	return this->max_x;
}

unsigned int Game::getMaxY() {
	return this->max_y;
}

unsigned int Game::getMaxEnemies() {
	return this->maxEnemies;
}

Player* Game::getPlayer() {
	return this->player;
}

std::vector<Enemy*> Game::getEnemies() {
	return this->enemies;
}

unsigned int Game::getEnemiesCount() {
	return this->enemies.size();
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
	pthread_mutex_lock(&writeMutex);

#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;


	SetConsoleCursorPosition(hConsole, pos);

	std::cout << text;
#else
	mvaddstr(y, x, text);
	refresh();
#endif

	pthread_mutex_unlock(&writeMutex);
}

void Game::erasePosition(unsigned int x, unsigned int y) {
	writeAtPosition(x, y, " ");
}

void Game::updatePoints() {
	char pointsText[15];
	sprintf(pointsText, "Points: %5i", points);
	writeAtPosition(0, this->max_y - 1, pointsText);
}

void Game::updateLifes() {
	char lifesText[10];
	sprintf(lifesText, "Lifes: %i", lifes);
	writeAtPosition(20, this->max_y - 1, lifesText);
}

void* Game::enemiesControl(void* context) {
	Game* c = (Game*) context;

    for (int i = 0; i < c->maxEnemies; i++) {
    	c->enemies.push_back(new Enemy(c, rand() % c->max_x, rand() % 5));
    	c-> writeAtPosition(c->enemies[i]->getX(), c->enemies[i]->getY(), "T");
    }

    do {
        for (int i = 0; i < c->getEnemiesCount(); i++) {
            if (!c->enemies[i]->isDead()) {
            	c->erasePosition(c->enemies[i]->getX(), c->enemies[i]->getY());
            	c->enemies[i]->moveX(rand() % 3 - 1);
                if (c->enemies[i]->getX() < 0) {
                	c->enemies[i]->setX(0);
                }
                if (c->enemies[i]->getX() >= c->max_x) {
                	c->enemies[i]->setX(c->max_x - 1);
                }
                c->enemies[i]->moveY(rand() % 3 - 1);
                if (c->enemies[i]->getY() < 0) {
                	c->enemies[i]->setY(0);
                }
                if (c->enemies[i]->getY() >= 5) {
                	c->enemies[i]->setY(4);
                }
                c->writeAtPosition(c->enemies[i]->getX(), c->enemies[i]->getY(), "T");
                c->enemies[i]->fireBullet();
#ifdef _WIN32
                Sleep(250);
#else
                usleep(250000);
#endif
            }
        }
    } while(true);

    pthread_exit(NULL);
    return 0;
}
