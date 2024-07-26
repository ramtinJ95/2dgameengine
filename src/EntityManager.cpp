#include "EntityManager.h"
#include "Entity.h"

EntityManager::EntityManager() {}

void EntityManager::update() {
  for (auto e : m_entitiesToAdd){
      m_entities.push_back(e);
      m_entityMap[e->tag()].push_back(e);
  }
  m_entitiesToAdd.clear();
  // remove dead entities from the vector of all entities
  removeDeadEntities(m_entities);

  // remove dead entities from each vector in the entity map
  // C++20 way of iterating through [key, value] pairs in a map
  for (auto &[tag, entityVec] : m_entityMap) {
    removeDeadEntities(entityVec);
  }
}

void EntityManager::removeDeadEntities(EntityVec &vec) {
  // todo: remove all dead entities from the input vector
  //       this is called by the update() function
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag) {
  auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
  m_entitiesToAdd.push_back(entity);

  return entity;
}

const EntityVec &EntityManager::getEntities() { return m_entities; }

const EntityVec &EntityManager::getEntities(const std::string &tag) {
  return m_entityMap[tag];
}

const std::map<std::string, EntityVec> &EntityManager::getEntityMap() {
  return m_entityMap;
}
