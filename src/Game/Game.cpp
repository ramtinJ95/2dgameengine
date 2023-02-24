#include "Game.h" // no need for angle brackets since the header that we want is in this same folder
#include <SDL2/SDL_image.h>
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_video.h"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <iostream>
#include "../ECS/ECS.h"
#include "../Logger/Logger.h"

Game::Game() {
    isRunning = false;
    registry = new Registry();
    Logger::Log("Game constructor called");
}

Game::~Game(){
    Logger::Log("Game destructor called");
}

void Game::Initialize(){
   if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Logger::Err("Error intitializing SDL");
        return;
    }
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = 800; 
    windowHeight = 600; 
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_BORDERLESS
        );
    if(!window) {
        Logger::Err("Error creating SDL window");
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer) {
        Logger::Err("Error creating SDL rendere");
        return;
    }
    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    isRunning = true;
}


void Game::ProcessInput(){
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) { // while loop because many things can happen in the loop and we want to keep listening for input events
        switch (sdlEvent.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if(sdlEvent.key.keysym.sym == SDLK_ESCAPE){
                    isRunning = false;
                }
                break;
        }
    }
}

glm::vec2 playerPos;
glm::vec2 playerVelocity;

void Game::Setup() {
    playerPos = glm::vec2(10.0, 20.0);
    playerVelocity = glm::vec2(100.0, 10.0);

    Entity tank = registry->CreateEntity();
    Entity truck= registry->CreateEntity();

}

void Game::Update(){
    // if we would want to have uncapped fps for a game just comment out the 4 lines below.
   int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
        SDL_Delay(timeToWait);
    }

    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;
    millisecsPreviousFrame = SDL_GetTicks(); // store "previous" frame
 
    playerPos.x += playerVelocity.x * deltaTime;
    playerPos.y += playerVelocity.y * deltaTime;
}

void Game::Render(){
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    // Loads a PNG texture
    SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    // What is the destination rect that we want to place our textur What is the destination rect that we want to place our texture

    SDL_Rect destinationRect = {
        static_cast<int>(playerPos.x), 
        static_cast<int>(playerPos.y), 
        32, 
        32};
    SDL_RenderCopy(renderer, texture, NULL, &destinationRect);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer); // this has dubbel buffer rendering built in
}

void Game::Run(){
    Setup();

    while(isRunning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::Destroy(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
