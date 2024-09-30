#include "EntityManager.hpp"

namespace acc {

EntityManager::EntityManager(void){
	for(int i = 0; i < MAX_ENTITIES; i++)
		entity_queue.push(i);
}

Entity EntityManager::getNextEntity(void){
	Entity front = entity_queue.front();

	entity_queue.pop();

	managed_entities.insert(front);

	return front;
}
		
void EntityManager::removeEntity(Entity entity){
	entity_queue.push(entity);
	managed_entities.erase(entity);
}

bool EntityManager::isEntityManaged(Entity entity){
	return managed_entities.find(entity) != managed_entities.end();
}

};
