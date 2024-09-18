#ifndef BEHAVIOR_COMPONENT_HPP
#define BEHAVIOR_COMPONENT_HPP

#include "Scene.hpp"
#include "Game.hpp"
#include "Context.hpp"

namespace acc {

class BehaviorFunction {
	public:
		BehaviorFunction(void);

		virtual void onCreate(void);
		virtual void onUpdate(void);
		virtual void onCollision(Entity other);

		bool hasCreated(void);
		void setAsCreated(void);
		virtual ~BehaviorFunction(void) = default;

		void setScene(Scene *scene);
		void setEntity(Entity entity);

	protected:
		Scene * getScene(void);
		Game * getGame(void);
		Context * getContext(void);

		template <typename T>
		T& getComponent(void){
			return scene->getComponentManager()->getComponent<T>(entity);
		}

		template <typename T>
		bool hasComponent(void){
			return scene->getComponentManager()->hasComponent<T>(entity);
		}

		template <typename T>
		T& getComponent(Entity id){
			return scene->getComponentManager()->getComponent<T>(id);
		}

		template <typename T>
		bool hasComponent(Entity id){
			return scene->getComponentManager()->hasComponent<T>(id);
		}

		/* search by a check function */
		template <typename T>
		std::vector<Entity> find(bool (*check)(T&)){
			std::vector<Entity> found;
			auto arr = scene->getComponentManager()->getComponentArray<T>();

			for(size_t i = 0; i < arr->getSize(); i++){
				if(check(arr->atIndex(i))){
					found.push_back(arr->indexToEntity(i));
				}
			}

			return found;
		}
	
	private:
		Scene *scene;
		Entity entity;

		bool has_created;
};

struct BehaviorComponent {
	BehaviorFunction *behavior_function;

	BehaviorComponent();
	void setFunction(BehaviorFunction *behavior_function);
	~BehaviorComponent(void);
};

};

#endif
