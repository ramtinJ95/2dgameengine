
LANG_STD = -std=c++17  
COMPILER_FLAGS = -Wall  
INCLUDE_PATH = -I"./libs/" -I"/usr/local/include"
SRC_FILES = ./src/*.cpp \
	    ./src/Game/*.cpp \
	    ./src/Logger/*.cpp \
            ./src/ECS/*.cpp
LINKER_FLAGS = -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -llua  
build:
	g++ $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATH) $(SRC_FILES) $(LINKER_FLAGS) -o gameengine

run:
	./gameengine

clean:
	rm gameengine
