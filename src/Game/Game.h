#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "../ECS/ECS.h"
const int FPS = 600;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
    private:
        bool isRunning;
        int millisecsPreviousFrame = 0;
        SDL_Window* window; 
        SDL_Renderer* renderer;
        
        Registry* registry;

    public:
        Game(); // constructor
        ~Game(); // this is a destructor
        void Initialize();
        void Setup();
        void Run();
        void Update();
        void Render();
        void Destroy();
        void ProcessInput();
        int windowWidth;
        int windowHeight;

};
#endif
