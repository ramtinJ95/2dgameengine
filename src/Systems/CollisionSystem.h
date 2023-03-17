#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Logger/Logger.h"
#include <string>

class CollisionSystem: public System {
public:
    CollisionSystem() {
	RequireComponent<TransformComponent>();
	RequireComponent<BoxColliderComponent>();
    }

    void Update() {
	auto entities = GetSystemEntities();

	for (auto i = entities.begin(); i != entities.end(); i++) {
	    Entity a = *i;
	    for (auto j = i; j != entities.end(); j++) {
		Entity b = *j;
		if (a == b) {
		    continue;
		}
		bool collisionHappned = CheckAABBCollision(a, b);
		if(collisionHappned) {
		    Logger::Log("Entity " + std::to_string(a.GetId()) + " is colliding with entity " + std::to_string(b.GetId()));
		}
	    }
	}
    }
    bool CheckAABBCollision(Entity a, Entity b) {
	double aX = a.GetComponent<TransformComponent>().position.x + a.GetComponent<BoxColliderComponent>().offset.x;
	double aY = a.GetComponent<TransformComponent>().position.y + a.GetComponent<BoxColliderComponent>().offset.y;
	double aW = a.GetComponent<BoxColliderComponent>().width;
	double aH = a.GetComponent<BoxColliderComponent>().height;

	double bX = b.GetComponent<TransformComponent>().position.x + b.GetComponent<BoxColliderComponent>().offset.x;
	double bY = b.GetComponent<TransformComponent>().position.y + b.GetComponent<BoxColliderComponent>().offset.y;
	double bW = b.GetComponent<BoxColliderComponent>().width;
	double bH = b.GetComponent<BoxColliderComponent>().height;

	return (
	    aX < bX + bW &&
	    aX + aW > bX &&
	    aY < bY + bH &&
	    aY + aH > bY
	);
	
    }
};


#endif
