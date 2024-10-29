#include "EntityManager.hpp"

namespace acc {

EntityManager::EntityManager(void){
	next_entity = 0;
}

Entity EntityManager::getNextEntity(void){
	managed_entities.insert(next_entity);

	return next_entity++;
}
		
void EntityManager::removeEntity(Entity entity){
	managed_entities.erase(entity);
}

bool EntityManager::isEntityManaged(Entity entity){
	return managed_entities.find(entity) != managed_entities.end();
}

};
