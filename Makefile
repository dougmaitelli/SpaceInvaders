all: space

clean:
	-rm main.o space.exe space

space: main.o
	g++ -g -o space main.o

main.o: main.cpp
	g++ -c -g main.cpp
