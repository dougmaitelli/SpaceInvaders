#ifdef _WIN32
#include <conio.h>

#define KEY_LEFT 75
#define KEY_UP 72
#define KEY_RIGHT 77
#define KEY_DOWN 80
#else
#include <curses.h>
#endif

#include "Base/Game.h"
#include "Entities/Player.h"

int main()
{
#ifndef _WIN32
	initscr();
        keypad(stdscr, TRUE);
        nonl();
        cbreak();
        noecho();
        curs_set(0);
#endif

	Game* game = new Game();
	Player* player = game->getPlayer();

	int key;

	do {
		game->writeAtPosition(player->getX(), player->getY(), "A");

		key = getch();

		player->setDead(false);

		game->erasePosition(player->getX(), player->getY());

		switch (key) {
			case KEY_LEFT:
				if (player->getX() > 0) {
					player->moveX(-1);
				}
				break;
			case KEY_UP:
				if (player->getY() > 10) {
					player->moveY(-1);
				}
				break;
			case KEY_RIGHT:
				if (player->getX() < X_MAX-1) {
					player->moveX(1);
				}
				break;
			case KEY_DOWN:
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
