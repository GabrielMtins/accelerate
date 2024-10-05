#ifndef SCENE_HPP
#define SCENE_HPP

#include <queue>
#include "Resource.hpp"
#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"

namespace acc {

class Game;

class Scene {
	public:
		Scene(Game *game);
		Game * getGame(void);
		ComponentManager * getComponentManager(void);
		void update(void);
		/**
		 * Add the entity to the destroy queue, so that it
		 * can be destroyed at the end of the current update.
		 */
		void addToDestroyQueue(Entity entity);
		/**
		 * Returns true if the entity is still being managed (if
		 * the entity is still being used).
		 */
		bool isEntityManaged(Entity entity);
		virtual ~Scene();

	protected:
		/**
		 * Returns the next available Entity.
		 */
		Entity getNextEntity(void);
		/**
		 * Removes an entity and all its components.
		 * Prefer to use addToDestroyQueue(), because removing entities
		 * at the middle of the frame can cause serious problems, like an
		 * entity referencing another that is not being managed anymore.
		 */
		void removeEntity(Entity entity);

		template <typename T>
		T& getComponent(Entity entity){
			return component_manager->getComponent<T>(entity);
		}

		template <typename T>
		bool addComponent(Entity entity){
			return component_manager->addComponent<T>(entity);
		}

		template <typename T>
		bool addComponent(Entity entity, T component){
			return component_manager->addComponent<T>(entity, component);
		}

		template <typename T>
		bool removeComponent(Entity entity){
			return component_manager->removeComponent<T>(entity);
		}

		template <typename T>
		void registerComponent(void){
			component_manager->registerComponent<T>();
		}


		/**
		 * A pointer to a Game.
		 */
		Game *game;
		/**
		 * The entity manager. Prefer to not use it directly.
		 */
		EntityManager *entity_manager;
		/**
		 * The component manager. Prefer to not use it directly.
		 */
		ComponentManager *component_manager;
		/**
		 * The system manager. Prefer to not use it directly.
		 */
		SystemManager *system_manager;
		/**
		 * The queue of the entities to be destroyed at the end of the
		 * current update.
		 */
		std::queue<Entity> destroyed_entities;
};

};

#endif
