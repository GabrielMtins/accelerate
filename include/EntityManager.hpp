#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "global.hpp"
#include <queue>
#include <unordered_set>

namespace acc {

class EntityManager {
	public:
		EntityManager(void);
		Entity getNextEntity(void);
		void removeEntity(Entity entity);
		bool isEntityManaged(Entity entity);

	private:
		std::queue<Entity> entity_queue;
		std::unordered_set<Entity> managed_entities;
};

};

#endif
