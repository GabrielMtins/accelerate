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
		void addToDestroyQueue(Entity entity);
		virtual ~Scene();

	protected:
		Entity getNextEntity(void);
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


		Game *game;
		EntityManager *entity_manager;
		ComponentManager *component_manager;
		SystemManager *system_manager;
		std::queue<Entity> destroyed_entities;
};

};

#endif
