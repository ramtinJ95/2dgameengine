#include "Game.h" // no need for angle brackets since the header that we want is in this same folder
#include <SDL2/SDL_image.h>
#include "../Components/SpriteComponent.h"
#include <SDL2/SDL.h>
#include <fstream>
#include <glm/glm.hpp>
#include "../Systems/MovementSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderColliderSystem.h"
#include "../Systems//RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/RigidBodyComponent.h" // maybe have one header file with all components this could get bothersome.
#include "../Components/AnimationComponent.h"
#include <iostream>
#include <memory>
#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../Logger/Logger.h"

Game::Game() {
    isRunning = false;
    isDebug = false;
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
                if (sdlEvent.key.keysym.sym == SDLK_d) {
                    isDebug = !isDebug;
                }
                break;
        }
    }
}


void Game::Setup() {

    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<RenderColliderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    
    // Add assets to the asset store
    assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
    assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
    assetStore->AddTexture(renderer,"tilemap-image","./assets/tilemaps/jungle.png");
    assetStore->AddTexture(renderer,"chopper-image","./assets/images/chopper.png");
    assetStore->AddTexture(renderer,"radar-image","./assets/images/radar.png");
// TODO: rewrite this entire setup method to be much more flexible, but right now more systems needs to be implemented first
    int tileSize = 32;
    double tileScale = 1.0;
    int mapNumCols = 25;
    int mapNumRows =20;
    std::fstream mapFile;
    mapFile.open("./assets/tilemaps/jungle.map");

    for (int i = 0; i < mapNumRows; i++) {
        for (int j = 0; j < mapNumCols; j++) {
            char ch;
            mapFile.get(ch);
            int srcRectY = std::atoi(&ch) * tileSize;
            mapFile.get(ch);
            int srcRectX = std::atoi(&ch) * tileSize;
            mapFile.ignore();

            Entity tile = registry->CreateEntity();
            tile.AddComponent<TransformComponent>(glm::vec2(j * (tileScale * tileSize), i * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
            tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, srcRectX, srcRectY);
        }
    }
    mapFile.close();

    Entity tank = registry->CreateEntity();
    Entity truck = registry->CreateEntity();
    Entity chopper = registry->CreateEntity();
    Entity radar = registry->CreateEntity();
    // registry->AddComponent<TransformComponent>(tank, glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
    // registry->AddComponent<RigidBodyComponent>(tank, glm::vec2(50.0, 0.0));
    //
    // The Entity templates with the Registry class pointer in the Entity class allows me to change above code to
    // A syntax that feels more natural and looks more like unity stuff

    chopper.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
    chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 1);
    chopper.AddComponent<AnimationComponent>(2, 10, true);

    radar.AddComponent<TransformComponent>(glm::vec2(windowWidth - 74, 10.0), glm::vec2(1.0, 1.0), 0.0);
    radar.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 1);
    radar.AddComponent<AnimationComponent>(8, 8, true);

    tank.AddComponent<TransformComponent>(glm::vec2(500.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(-30.0, 0.0));
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 2);
    tank.AddComponent<BoxColliderComponent>(32, 32);

    truck.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
    truck.AddComponent<RigidBodyComponent>(glm::vec2(20.0, 0.0));
    truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 2);
    truck.AddComponent<BoxColliderComponent>(32, 32);

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
    registry->GetSystem<AnimationSystem>().Update();
    registry->GetSystem<CollisionSystem>().Update();
}

void Game::Render(){
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);
    
    registry->GetSystem<RenderSystem>().Update(renderer, assetStore);
    if (isDebug) {
        registry->GetSystem<RenderColliderSystem>().Update(renderer);
    }
    
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
