#ifndef PLAYER_H_
#define PLAYER_H_

class Game;

#include "SpaceShip.h"

class Player: public SpaceShip {
public:
	Player(Game*, int, int);
	virtual ~Player();
};

#endif /* PLAYER_H_ */
