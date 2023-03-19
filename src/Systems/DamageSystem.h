#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Events/CollisionEvent.h"
#include "../EventBus/EventBus.h"
#include "../Logger/Logger.h"
#include <memory>
#include <string>

class DamageSystem: public System {
public:
    DamageSystem() {
	RequireComponent<BoxColliderComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
	eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
    }

    void onCollision(CollisionEvent& event) {
	Logger::Log("did we get in here");
	Logger::Log("Damage system received an even collison between entites " + std::to_string(event.a.GetId()) + " and " + std::to_string(event.b.GetId())); 
	event.a.Kill();
	event.b.Kill();

    }

    void Update() {
    }
};


#endif
