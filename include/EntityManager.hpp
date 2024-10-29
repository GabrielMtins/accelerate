#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "global.hpp"
#include <queue>
#include <unordered_set>

namespace acc {

class EntityManager {
	public:
		EntityManager(void);
		/**
		 * Returns the next Entity available.
		 */
		Entity getNextEntity(void);
		/**
		 * Removes the entity from the scene and put back into
		 * the queue of entities.
		 */
		void removeEntity(Entity entity);
		/**
		 * Returns true if the entity is being managed (if that entity id
		 * is being used). Returns false, otherwise.
		 */
		bool isEntityManaged(Entity entity);

	private:
		size_t next_entity;
		std::unordered_set<Entity> managed_entities;
};

};

#endif
