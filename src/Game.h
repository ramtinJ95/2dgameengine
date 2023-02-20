#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

class Game {
    private:
        bool isRunning;
        SDL_Window* window; 
        SDL_Renderer* renderer;

    public:
        Game(); // constructor
        ~Game(); // this is a destructor
        void Initialize();
        void Run();
        void Update();
        void Render();
        void Destroy();
        void ProcessInput();
        int windowWidth;
        int windowHeight;

};
#endif
