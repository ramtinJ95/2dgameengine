#ifndef RENDERSYSTEM_H 
#define RENDERSYSTEM_H 

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include <SDL2/SDL.h>
#include "../AssetStore/AssetStore.h"
#include <memory>

class RenderSystem: public System {
    public:
	RenderSystem() {
	    RequireComponent<TransformComponent>();
	    RequireComponent<SpriteComponent>();
	}
	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
	    for (auto entity: GetSystemEntites()) {
		const auto transform = entity.GetComponent<TransformComponent>();
		const auto sprite = entity.GetComponent<SpriteComponent>();

	        SDL_Rect srcRect = sprite.srcRect;  

		SDL_Rect destRect = {
		    static_cast<int>(transform.position.x),
		    static_cast<int>(transform.position.y),
		    static_cast<int>(sprite.width * transform.scale.x),
		    static_cast<int>(sprite.height * transform.scale.y)
	        };

	        SDL_RenderCopyEx(
		    renderer, 
		    assetStore->GetTexture(sprite.assetId), 
		    &srcRect, 
		    &destRect,
		    transform.rotation,
		    NULL,
		    SDL_FLIP_NONE
	        );

	    }
    }
};


#endif
