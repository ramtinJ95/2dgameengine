build:
	g++ -Wall -std=c++17 -I"./libs/" -I"/usr/local/include" src/*.cpp -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -llua -o gameengine

run:
	./gameengine

clean:
	rm gameengine
