#ifndef SPACESHIP_H_
#define SPACESHIP_H_

class Game;

class SpaceShip {
private:
	Game* game;
	int player_x, player_y, player_dir;
	bool dead;
public:
	SpaceShip(Game*, int, int, int);
	virtual ~SpaceShip();
	Game* getGame();
	int getX();
	void setX(int);
	void moveX(int);
	int getY();
	void setY(int);
	void moveY(int);
	void setDead(bool);
	bool isDead();
	void fireBullet();
};

#endif /* SPACESHIP_H_ */
