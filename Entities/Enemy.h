#ifndef ENEMY_H_
#define ENEMY_H_

class Game;

#include "SpaceShip.h"

class Enemy: public SpaceShip {
public:
	Enemy(Game*, int, int);
	virtual ~Enemy();
};

#endif /* ENEMY_H_ */
