CC = g++
CFLAGS  = -g
ODIR = build

default: clean preBuild build

preBuild: 
	mkdir $(ODIR)

build:  main.o Game.o Bullet.o Enemy.o Player.o SpaceShip.o
	$(CC) $(CFLAGS) -o space $(ODIR)/main.o $(ODIR)/Game.o $(ODIR)/Bullet.o $(ODIR)/Enemy.o $(ODIR)/Player.o $(ODIR)/SpaceShip.o -lpthread -lcurses

main.o:  main.cpp Base/Game.h Entities/Player.h 
	$(CC) $(CFLAGS) -c main.cpp -o $(ODIR)/main.o

Game.o:  Base/Game.cpp Entities/Player.h Entities/Enemy.h
	$(CC) $(CFLAGS) -c Base/Game.cpp -o $(ODIR)/Game.o

Bullet.o:  Entities/Bullet.cpp Entities/Player.h Entities/Enemy.h
	$(CC) $(CFLAGS) -c Entities/Bullet.cpp -o $(ODIR)/Bullet.o

Enemy.o:  Entities/Enemy.cpp Entities/SpaceShip.h
	$(CC) $(CFLAGS) -c Entities/Enemy.cpp -o $(ODIR)/Enemy.o
	
Player.o:  Entities/Player.cpp Entities/SpaceShip.h
	$(CC) $(CFLAGS) -c Entities/Player.cpp -o $(ODIR)/Player.o
	
SpaceShip.o:  Entities/SpaceShip.cpp Entities/Bullet.h
	$(CC) $(CFLAGS) -c Entities/SpaceShip.cpp -o $(ODIR)/SpaceShip.o

clean: 
	$(RM) -r $(ODIR) *~ space
