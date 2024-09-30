#ifndef BEHAVIOR_COMPONENT_HPP
#define BEHAVIOR_COMPONENT_HPP

#include "Scene.hpp"
#include "Game.hpp"
#include "Context.hpp"
#include "Vec3.hpp"

#include <memory>

namespace acc {

class BehaviorFunction {
	public:
		BehaviorFunction(void);

		virtual void onCreate(void);
		virtual void onUpdate(void);
		virtual void onCollision(Entity other);
		virtual void onMessage(Entity sender, std::string message);

		bool hasCreated(void);
		void setAsCreated(void);
		virtual ~BehaviorFunction(void) = default;

		void setScene(Scene *scene);
		void setEntity(Entity entity);

		Scene * getScene(void);

	protected:
		Game * getGame(void);
		Context * getContext(void);

		bool getKeyUp(std::string key);
		bool getKeyDown(std::string key);
		bool getKey(std::string key);

		bool getMouseButtonUp(std::string key);
		bool getMouseButtonDown(std::string key);
		bool getMouseButton(std::string key);

		bool isEntityManaged(Entity entity);

		bool raycast(Vec3 origin, Vec3 direction, uint32_t layer_mask, Entity *return_entity, Vec3 *return_intersection);
		bool sendMessage(Entity receiver, std::string message);

		Vec3 getMousePosition(void);

		float getDeltaTime(void);
		uint64_t getTicks(void);

		void destroy(Entity entity);
		void destroy(void);

		template <typename T>
		T& getComponent(void){
			return component_manager->getComponent<T>(entity);
		}

		template <typename T>
		bool hasComponent(void){
			return component_manager->hasComponent<T>(entity);
		}

		template <typename T>
		T& getComponent(Entity id){
			return component_manager->getComponent<T>(id);
		}

		template <typename T>
		bool hasComponent(Entity id){
			return component_manager->hasComponent<T>(id);
		}

		/* search by a check function */
		template <typename T, typename Functor>
		std::vector<Entity> find(Functor check){
			std::vector<Entity> found;
			auto arr = component_manager->getComponentArray<T>();

			for(size_t i = 0; i < arr->getSize(); i++){
				if(check(arr->atIndex(i))){
					found.push_back(arr->indexToEntity(i));
				}
			}

			return found;
		}
	
	private:
		Scene *scene;
		Game *game;
		Context *context;
		ComponentManager *component_manager;

		Entity entity;

		bool has_created;
};

struct BehaviorComponent {
	std::shared_ptr<BehaviorFunction> behavior_function;

	BehaviorComponent();
	void setFunction(std::shared_ptr<BehaviorFunction> behavior_function);
	~BehaviorComponent(void);
};

};

#endif
