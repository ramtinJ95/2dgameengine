#include "ECS.h"
#include "../Logger/Logger.h"
#include "algorithm"
#include <__algorithm/remove_if.h>

int IComponent::nextId = 0;

int Entity::GetId() const {
    return id;
}

void Entity::Kill() {
    registry->KillEntity(*this);
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
std::vector<Entity> System::GetSystemEntities() const {
    return entities;
}
const Signature& System::GetComponentSignature() const {
    return componentSignature;
}

Entity Registry::CreateEntity() {
    int entityId;
    if (freeIds.empty()) {
        entityId = numEntitets++;
        if(entityId >= entityComponentSignatures.size()) {
            entityComponentSignatures.resize(entityId + 1); // TODO this is wasting memory potentially.
        }
    } else {
        entityId = freeIds.front();
        freeIds.pop_front();
    }

    Entity entity(entityId);
    entity.registry = this; // we are inte Registry namespace, i.e Registry class. So the this keyword is the object pointer.
    entitiesToBeAdded.insert((entity));


    Logger::Log("Entity created with id = " + std::to_string(entityId));

    return entity;
}

void Registry::KillEntity(Entity entity) {
    entitesToBeKilled.insert(entity);
}

void Registry::RemoveEntityFromSystems(Entity entity) {
    for (auto system: systems) {
        system.second->RemoveEntityFromSystem(entity);
    }
}

void Registry::AddEntityToSystems(Entity entity){
    const auto entityId = entity.GetId();
    const auto& entityComponentSignature = entityComponentSignatures[entityId];

    for (auto& system: systems) {
        const auto& systemComponentSignature = system.second->GetComponentSignature();
        // bitwise AND here since they are two arrays of binary values, and it will only be true
        // if they are identical. 
        bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

        if(isInterested) {
            system.second->AddEntityToSystem(entity);
        }
    }
}

void Registry::Update() {
    for(auto entity : entitiesToBeAdded) {
        AddEntityToSystems(entity);
    }
    entitiesToBeAdded.clear();
    for (auto entity: entitesToBeKilled) {
        RemoveEntityFromSystems(entity);
        
        entityComponentSignatures[entity.GetId()].reset();

        freeIds.push_back(entity.GetId());
    }
    entitesToBeKilled.clear();
}
