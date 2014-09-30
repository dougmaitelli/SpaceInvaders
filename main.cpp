#include <iostream>
#include <stdio.h>
#include <conio.h>

#include "Game.h"
#include "Player.h"

using namespace std;

int main()
{
	Game* game = new Game();
	Player* player = game->getPlayer();

	int key;

	do {
		game->writeAtPosition(player->getX(), player->getY(), "A");

		key = getch();

		player->setDead(false);

		game->erasePosition(player->getX(), player->getY());

		switch (key) {
			case 75:
				if (player->getX() > 0) {
					player->moveX(-1);
				}
				break;
			case 72:
				if (player->getY() > 10) {
					player->moveY(-1);
				}
				break;
			case 77:
				if (player->getX() < X_MAX-1) {
					player->moveX(1);
				}
				break;
			case 80:
				if (player->getY() < Y_MAX-1) {
					player->moveY(1);
				}
				break;
			case 32:
				player->fireBullet();
				break;
			default:
				break;
		}

		if (game->getLifes() <= 0) {
			break;
		}
	} while(key != 27);

    return 0;
}
