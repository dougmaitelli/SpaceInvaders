#ifndef GAME_H_
#define GAME_H_

class Player;
class Enemy;

#include <pthread.h>

#ifdef _WIN32
#define X_MAX 80
#define Y_MAX 24
#endif

#define ENEMIES_MAX 500

class Game {
private:
	unsigned int max_x;
	unsigned int max_y;
	unsigned int maxEnemies;

	Player* player;
	Enemy* enemies[ENEMIES_MAX];

	unsigned int lifes;
	int points;

	pthread_mutex_t writeMutex;
public:
	Game(unsigned int, unsigned int);
	virtual ~Game();
	unsigned int getMaxX();
	unsigned int getMaxY();
	unsigned int getMaxEnemies();
	Player* getPlayer();
	Enemy** getEnemies();
	unsigned int getEnemiesCount();
	unsigned int getLifes();
	void setLifes(int);
	void changeLifes(int);
	int getPoints();
	void setPoints(int);
	void changePoints(int);
	void writeAtPosition(unsigned int, unsigned int, const char*);
	void erasePosition(unsigned int, unsigned int);
	void updatePoints();
	void updateLifes();
	static void* enemiesControl(void*);
};

#endif /* GAME_H_ */
