#include "Bullet.h"

#ifdef _WIN32
#include <conio.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>

#include <pthread.h>

#include "../Base/Game.h"
#include "Player.h"
#include "Enemy.h"

Bullet::Bullet(SpaceShip* spaceShip, int bullet_x, int bullet_y, int bullet_dir) {
	this->spaceShip = spaceShip;
	this->bullet_x = bullet_x;
	this->bullet_y = bullet_y;
	this->bullet_dir = bullet_dir;
}

Bullet::~Bullet() {
	// TODO Auto-generated destructor stub
}

void* Bullet::bulletExecution(void* context) {
	Bullet* c = (Bullet*) context;

	Game* game = c->spaceShip->getGame();

    bool hit = false;

    do {
    	if (c->bullet_dir == 1) {
    		c->bullet_y--;
		} else {
			c->bullet_y++;
		}

        if (c->bullet_dir == 1) {
        	Enemy** enemies = game->getEnemies();

            for (unsigned int i = 0; i < game->getEnemiesCount(); i++) {
                if (!enemies[i]->isDead() and enemies[i]->getX() == c->bullet_x and enemies[i]->getY() == c->bullet_y) {
                     enemies[i]->setDead(true);
                     game->changePoints(10);
                     game->updatePoints();
                     hit = true;
                     break;
                }
            }
        }

        Player* player = game->getPlayer();

        if (!player->isDead()) {
            if ((player->getX() == c->bullet_x) and (player->getY() == c->bullet_y)) {
                game->changeLifes(-1);
                game->changePoints(-20);
                game->updatePoints();
                game->updateLifes();
				hit = true;
                player->setDead(true);
            }
        }

        if (hit) {
        	printf("\a");
        	game->writeAtPosition(c->bullet_x, c->bullet_y, "O");
#ifdef _WIN32
            Sleep(30);
#else
            sleep(30);
#endif
            game->erasePosition(c->bullet_x, c->bullet_y);
            break;
        } else {
        	game->writeAtPosition(c->bullet_x, c->bullet_y, "|");
#ifdef _WIN32
            Sleep(50);
#else
            sleep(50);
#endif
            game->erasePosition(c->bullet_x, c->bullet_y);
        }
    } while((c->bullet_y > 0) and (c->bullet_y < 24 - 1));

    pthread_exit(NULL);
    return 0;
}
