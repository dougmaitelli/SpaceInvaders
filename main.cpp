#include <iostream>
#include <stdio.h>
#include <conio.h>

#include <pthread.h>

#define X_MAX 80
#define Y_MAX 24
#define ENEMIES_MAX 30

using namespace std;

int player_x = 40, player_y = 23;
static bool dead = false;
int inimigos[ENEMIES_MAX][2];
unsigned int lifes = 5;
int points = 1;

pthread_mutex_t writeMutex = PTHREAD_MUTEX_INITIALIZER;

void writeAtPosition(unsigned int x, unsigned int y, const char* text)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;

	pthread_mutex_lock(&writeMutex);
	SetConsoleCursorPosition(hConsole, pos);
	cout << text;
	pthread_mutex_unlock(&writeMutex);
}

void erasePosition(unsigned int x, unsigned int y)
{
	writeAtPosition(x, y, " ");
}

void updatePoints()
{
	char pointsText[15];
	sprintf(pointsText, "Points: %5i", points);
	writeAtPosition(0, 24, pointsText);
}

void updateLifes()
{
	char lifesText[10];
	sprintf(lifesText, "Lifes: %i", lifes);
	writeAtPosition(20, 24, lifesText);
}

struct fireBullet_arg_struct {
    int x;
    int y;
    int dir;
};

void *fireBullet(void *arguments)
{
	struct fireBullet_arg_struct *args = (struct fireBullet_arg_struct *) arguments;

    int x = args->x;
    int y = args->y;
    int dir = args->dir;

    bool quebra = false;

    if (dir == 1) {
        y--;
    } else {
        y++;
    }

    do {
        if (dir == 1) {
            for (int i = 0; i <= ENEMIES_MAX; i++) {
                if ((inimigos[i][1] == x) and (inimigos[i][2] == y)) {
                     inimigos[i][1] = -1;
                     inimigos[i][2] = -1;
                     points += 10;
					 updatePoints();
                     quebra = true;
                     break;
                }
            }
        }
        if (dead == false) {
            if ((player_x == x) and (player_y == y)) {
                printf("\a");
                lifes -= 1;
                points -= 20;
                updatePoints();
                updateLifes();
				quebra = true;
                dead = true;
            }
        }
        if (quebra) {
        	writeAtPosition(x, y, "O");
            Sleep(30);
            erasePosition(x, y);
            break;
        } else {
        	writeAtPosition(x, y, "|");
            Sleep(50);
            erasePosition(x, y);
        }
        if (dir == 1) {
            y--;
        } else {
            y++;
        }
        if ((y > Y_MAX-1) or (y < 0)) {
            break;
        }
    } while(true);

    pthread_exit(NULL);
    return 0;
}

void *enemiesControl(void*)
{
    for (int i = 0; i <= ENEMIES_MAX; i++) {
        inimigos[i][1] = rand()%X_MAX;
        inimigos[i][2] = rand()%5;
        writeAtPosition(inimigos[i][1], inimigos[i][2], "T");
    }
    do {
        for (int i = 0; i <= ENEMIES_MAX; i++) {
            if (inimigos[i][1] > -1) {
                erasePosition(inimigos[i][1], inimigos[i][2]);
                inimigos[i][1] += rand()%3-1;
                if (inimigos[i][1] < 0) {
                    inimigos[i][1] = 0;
                }
                if (inimigos[i][1] >= X_MAX) {
                    inimigos[i][1] = X_MAX-1;
                }
                inimigos[i][2] += rand()%3-1;
                if (inimigos[i][2] < 0) {
                    inimigos[i][2] = 0;
                }
                if (inimigos[i][2] >= 5) {
                    inimigos[i][2] = 4;
                }
                writeAtPosition(inimigos[i][1], inimigos[i][2], "T");
                pthread_t bulletThread;
                struct fireBullet_arg_struct args;
				args.x = inimigos[i][1];
				args.y = inimigos[i][2];
				args.dir = 2;
                pthread_create(&bulletThread, NULL, fireBullet, (void *)&args);
                Sleep(250);
            }
        }
    } while(true);

    pthread_exit(NULL);
    return 0;
}

int main()
{
    int tecla;

    updatePoints();
    updateLifes();

    pthread_t enemiesThread;
    pthread_create(&enemiesThread, NULL, enemiesControl, NULL);

    do {
    	writeAtPosition(player_x, player_y, "A");

    	tecla = getch();

        dead = false;

        switch (tecla) {
            case 75:
                if (player_x > 0) {
                    erasePosition(player_x, player_y);
                    player_x -= 1;
                }
                break;
            case 72:
                if (player_y > 10) {
                    erasePosition(player_x, player_y);
                    player_y -= 1;
                }
                break;
            case 77:
                if (player_x < X_MAX-1) {
                    erasePosition(player_x, player_y);
                    player_x += 1;
                }
                break;
            case 80:
                if (player_y < Y_MAX-1) {
                    erasePosition(player_x, player_y);
                    player_y += 1;
                }
                break;
            case 32:
            	pthread_t playerBulletThread;
            	struct fireBullet_arg_struct args;
				args.x = player_x;
				args.y = player_y;
				args.dir = 1;
                pthread_create(&playerBulletThread, NULL, fireBullet, (void *)&args);
                break;
            default:
                break;
        }
        if (lifes <= 0) {
            break;
        }
    } while(tecla != 27);

    pthread_mutex_destroy(&writeMutex);

    return 0;
}
