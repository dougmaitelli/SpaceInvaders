SOURCES = main.cpp Base/Game.cpp Entities/Bullet.cpp Entities/Enemy.cpp Entities/Player.cpp Entities/SpaceShip.cpp

all: space

clean:
	-rm main.o space.exe space

space: main.o
	g++ -g -o space main.o -lcurses

main.o: main.cpp
	g++ -c -g $(SOURCES)
