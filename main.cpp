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

pthread_t thread[2];
pthread_mutex_t MutexEscreve;

void Apaga(int x, int y)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {0, 0};

    pthread_mutex_lock(&MutexEscreve);
    pos = (COORD){x, y};
    SetConsoleCursorPosition(hConsole, pos);
    cout << " ";
    pthread_mutex_unlock(&MutexEscreve);
}

PVOID Tiro(PVOID z)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {0, 0};

    int x = (((int)z-(int)z%1000)/1000);
    int y = (((int)z%1000-(int)z%10)/10);
    int dir = (int)z%10;

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
                     pthread_mutex_lock(&MutexEscreve);
                     pos = (COORD){0, 24};
                     SetConsoleCursorPosition(hConsole, pos);
                     printf("Points: %5i", points);
                     pthread_mutex_unlock(&MutexEscreve);
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
                pthread_mutex_lock(&MutexEscreve);
                pos = (COORD){0, 24};
                SetConsoleCursorPosition(hConsole, pos);
                printf("Points: %5i", points);
                pos = (COORD){20, 24};
                SetConsoleCursorPosition(hConsole, pos);
                cout << "Lifes: " << lifes;
                pthread_mutex_unlock(&MutexEscreve);
                quebra = true;
                dead = true;
            }
        }
        pthread_mutex_lock(&MutexEscreve);
        pos = (COORD){x, y};
        SetConsoleCursorPosition(hConsole, pos);
        if (quebra) {
            cout << "O";
            pthread_mutex_unlock(&MutexEscreve);
            Sleep(30);
            Apaga(x, y);
            break;
        } else {
            cout << "|";
            pthread_mutex_unlock(&MutexEscreve);
            Sleep(50);
            Apaga(x, y);
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

PVOID Inimigos(PVOID id)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {0, 0};

    for (int i = 0; i <= ENEMIES_MAX; i++) {
        inimigos[i][1] = rand()%X_MAX;
        inimigos[i][2] = rand()%5;
        pthread_mutex_lock(&MutexEscreve);
        pos = (COORD){inimigos[i][1], inimigos[i][2]};
        SetConsoleCursorPosition(hConsole, pos);
        cout << "T";
        pthread_mutex_unlock(&MutexEscreve);
    }
    do {
        for (int i = 0; i <= ENEMIES_MAX; i++) {
            if (inimigos[i][1] > -1) {
                Apaga(inimigos[i][1], inimigos[i][2]);
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
                pthread_mutex_lock(&MutexEscreve);
                pos = (COORD){inimigos[i][1], inimigos[i][2]};
                SetConsoleCursorPosition(hConsole, pos);
                cout << "T";
                pthread_mutex_unlock(&MutexEscreve);
                pthread_create(&thread[1], NULL, Tiro, (PVOID)(inimigos[i][1]*1000+inimigos[i][2]*10+2));
                Sleep(250);
            }
        }
    } while(true);
    pthread_exit(NULL);
}

int main()
{
    int tecla;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {0, 0};

    MutexEscreve = PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_lock(&MutexEscreve);
    pos = (COORD){0, 24};
    SetConsoleCursorPosition(hConsole, pos);
    printf("Points: %5i", points);
    pos = (COORD){20, 24};
    SetConsoleCursorPosition(hConsole, pos);
    cout << "Lifes: " << lifes;
    pthread_mutex_unlock(&MutexEscreve);

    pthread_create(&thread[2], NULL, Inimigos, (PVOID)1);

    do {
        pthread_mutex_lock(&MutexEscreve);
        pos = (COORD){player_x, player_y};
        SetConsoleCursorPosition(hConsole, pos);

        cout << "A";
        pthread_mutex_unlock(&MutexEscreve);

        tecla = getch();

        dead = false;

        switch (tecla) {
            case 75:
                if (player_x > 0) {
                    Apaga(player_x, player_y);
                    player_x -= 1;
                }
                break;
            case 72:
                if (player_y > 10) {
                    Apaga(player_x, player_y);
                    player_y -= 1;
                }
                break;
            case 77:
                if (player_x < X_MAX-1) {
                    Apaga(player_x, player_y);
                    player_x += 1;
                }
                break;
            case 80:
                if (player_y < Y_MAX-1) {
                    Apaga(player_x, player_y);
                    player_y += 1;
                }
                break;
            case 32:
                pthread_create(&thread[1], NULL, Tiro, (PVOID)(player_x*1000+player_y*10+1));
                break;
            default:
                break;
        }
        if (lifes <= 0) {
            break;
        }
    } while(tecla != 27);
    return 0;
}
