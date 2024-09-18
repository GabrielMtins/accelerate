#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "global.hpp"
#include <queue>

namespace acc {

class EntityManager {
	public:
		EntityManager(void);

		Entity getNextEntity(void);
		
		void removeEntity(Entity entity);

	private:
		std::queue<Entity> entity_queue;
};

};

#endif
