CC = g++
CFLAGS  = -g

default: space

space:  main.o Game.o Bullet.o Enemy.o Player.o SpaceShip.o
	$(CC) $(CFLAGS) -o space main.o Game.o Bullet.o Enemy.o Player.o SpaceShip.o -lcurses

main.o:  main.cpp Base/Game.h Entities/Player.h 
	$(CC) $(CFLAGS) -c main.cpp

Game.o:  Base/Game.cpp Entities/Player.h Entities/Enemy.h
	$(CC) $(CFLAGS) -c Base/Game.cpp

Bullet.o:  Entities/Bullet.cpp Entities/Player.h Entities/Enemy.h
	$(CC) $(CFLAGS) -c Entities/Bullet.cpp

Enemy.o:  Entities/Enemy.cpp Entities/SpaceShip.h
	$(CC) $(CFLAGS) -c Entities/Enemy.cpp
	
Player.o:  Entities/Player.cpp Entities/SpaceShip.h
	$(CC) $(CFLAGS) -c Entities/Player.cpp
	
SpaceShip.o:  Entities/SpaceShip.cpp Entities/Bullet.h
	$(CC) $(CFLAGS) -c Entities/SpaceShip.cpp

clean: 
	$(RM) space *.o *~