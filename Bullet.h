#ifndef BULLET_H_
#define BULLET_H_

class SpaceShip;

class Bullet {
private:
	SpaceShip* spaceShip;
	int bullet_x;
	int bullet_y;
	int bullet_dir;
public:
	Bullet(SpaceShip*, int, int, int);
	virtual ~Bullet();
	static void* bulletExecution(void*);
};

#endif /* BULLET_H_ */
