#include "Game.h" // no need for angle brackets since the header that we want is in this same folder
#include <SDL2/SDL_image.h>
#include "../Components/SpriteComponent.h"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "../Systems/MovementSystem.h"
#include "../Systems//RenderSystem.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h" // maybe have one header file with all components this could get bothersome.
#include <iostream>
#include <memory>
#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../Logger/Logger.h"

Game::Game() {
    isRunning = false;
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();
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


void Game::Setup() {

    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    
    // Add assets to the asset store
    assetStore->AddTexture(renderer, "tank-image", "./assets/image/tank-panther-right.png");
    assetStore->AddTexture(renderer, "truck-image", "./assets/image/truck-ford-right.png");

    Entity tank = registry->CreateEntity();
    // registry->AddComponent<TransformComponent>(tank, glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
    // registry->AddComponent<RigidBodyComponent>(tank, glm::vec2(50.0, 0.0));
    //
    // The Entity templates with the Registry class pointer in the Entity class allows me to change above code to
    // A syntax that feels more natural and looks more like unity stuff
    tank.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(50.0, 0.0));
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32);
}

void Game::Update(){
    // if we would want to have uncapped fps for a game just comment out the 4 lines below.
   int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
        SDL_Delay(timeToWait);
    }

    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;
    millisecsPreviousFrame = SDL_GetTicks(); // store "previous" frame
 
    // Update the registry to process the enities that are wating to be created/deleted. 
    registry->Update();
    
    registry->GetSystem<MovementSystem>().Update(deltaTime);
}

void Game::Render(){
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);
    
    registry->GetSystem<RenderSystem>().Update(renderer, assetStore);
    
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
