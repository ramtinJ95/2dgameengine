#include "ECS.h"
#include "../Logger/Logger.h"
#include "algorithm"
#include <__algorithm/remove_if.h>
int Entity::GetId() const {
    return id;
}

void System::AddEntityToSystem(Entity entity) {
    entities.push_back(entity);
}
void System::RemoveEntityFromSystem(Entity entity) {
/* This for loop below is just to show what I would write in normal cases
 * but since this will lead to resize of the underlying array that the vector class uses
 * and this is inefficient, ofc this list will never be larger than 10k since at that point
 * the bottle-neck would be renderin probably, I will use the c++ way of Erase-remove idiom, see
 * my wiki for more explanation.
 *
 for (int i = 0; i <= entities.size(); i++) {
        if(entities[i].GetId() == entity.GetId()) {
            std::swap(entities[i], entities.back());
            entities.pop_back();
        }
    }
*/
    entities.erase(std::remove_if(
        entities.begin(), 
        entities.end(),
        [&entity](Entity otherEntity) {
            return entity == otherEntity;
        }),
        entities.end());

}
std::vector<Entity> System::GetSystemEntites() const {
    return entities;
}
const Signature& System::GetComponentSignature() const {
    return componentSignature;
}

Entity Registry::CreateEntity() {
    int entityId;
    entityId = numEntitets++;
    
    Entity entity(entityId);
    entitiesToBeAdded.insert((entity));

    Logger::Log("Entity created with id = " + std::to_string(entityId));

    return entity;

}

