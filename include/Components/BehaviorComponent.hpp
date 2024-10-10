#ifndef BEHAVIOR_COMPONENT_HPP
#define BEHAVIOR_COMPONENT_HPP

#include "Scene.hpp"
#include "Game.hpp"
#include "Context.hpp"
#include "Vec3.hpp"

#include <memory>

namespace acc {

/** 
 * This is the main behavior class to inherent from.
 * You should override the virtual functions.
 */
class BehaviorFunction {
	public:
		BehaviorFunction(void);

		/**
		 * The function that gets called when the entity
		 * is created
		 */
		virtual void onCreate(void);
		/**
		 * The function that gets called every frame.
		 */
		virtual void onUpdate(void);
		/**
		 * This function gets called every time this entity collides
		 * with another. The argument is the "other" entity who was
		 * collided.
		 */
		virtual void onCollision(Entity other);
		/**
		 * This functions gets called every time a entity sends
		 * a message to another. The first argument is a sender,
		 * the entity who sends the message. The second argument
		 * is the message sended.
		 */
		virtual void onMessage(Entity sender, const std::string& message);

		bool hasCreated(void);
		void setAsCreated(void);
		virtual ~BehaviorFunction(void) = default;

		void setScene(Scene *scene);
		void setEntity(Entity entity);

		/**
		 * Returns a pointer to the Scene class.
		 */
		Scene * getScene(void);

	protected:
		/**
		 * Returns a pointer to the Game class.
		 */
		Game * getGame(void);
		/**
		 * Returns a pointer to the Context class.
		 */
		Context * getContext(void);

		/**
		 * Returns true if the key was released at the same frame the
		 * function was called.
		 */
		bool getKeyUp(const std::string& key);
		/**
		 * Returns true if the key was pressed at the same frame
		 * the function was called.
		 */
		bool getKeyDown(const std::string& key);
		/**
		 * Returns true if the key is being pressed.
		 */
		bool getKey(const std::string& key);

		/**
		 * Returns true if the mouse button was released at the same frame
		 * the function was called.
		 */
		bool getMouseButtonUp(const std::string& key);
		/**
		 * Returns true if the mouse button was pressed at the same frame
		 * the function was called.
		 */
		bool getMouseButtonDown(const std::string& key);
		/**
		 * Returns true if the mouse button is being pressed.
		 */
		bool getMouseButton(const std::string& key);

		/**
		 * Returns true if the entity is still being managed
		 * by the Scene.
		 */
		bool isEntityManaged(Entity entity);

		/**
		 * Casts a ray and sets the return_entity to the first entity found.
		 * Returns true if the ray intersected something.
		 */
		bool raycast(Vec3 origin, Vec3 direction, uint32_t layer_mask, Entity *return_entity, Vec3 *return_intersection);
		/**
		 * Sends a message to a receiver.
		 */
		bool sendMessage(Entity receiver, const std::string& message);

		/**
		 * Returns a Vec3 containing the mouse position.
		 */
		Vec3 getMousePosition(void);

		/**
		 * Returns the time between frames in seconds.
		 */
		float getDeltaTime(void);
		/**
		 * Returns the time in milliseconds since the game was started.
		 */
		uint64_t getTicks(void);

		/**
		 * Destroys an entity.
		 */
		void destroy(Entity entity);
		/**
		 * Destroys the entity who called it.
		 */
		void destroy(void);

		/**
		 * Gets the component of the entity.
		 */
		template <typename T>
		T& getComponent(void){
			return component_manager->getComponent<T>(entity);
		}

		/**
		 * Returns true if the entity has the component T.
		 */
		template <typename T>
		bool hasComponent(void){
			return component_manager->hasComponent<T>(entity);
		}

		/**
		 * Gets the component of the entity.
		 */
		template <typename T>
		T& getComponent(Entity id){
			return component_manager->getComponent<T>(id);
		}

		/**
		 * Returns true if the entity has the component T.
		 */
		template <typename T>
		bool hasComponent(Entity id){
			return component_manager->hasComponent<T>(id);
		}

		/** 
		 * Search from all the components on the Scene.
		 * It should be used with a function.
		 */
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

	BehaviorComponent(void);
	BehaviorComponent(std::shared_ptr<BehaviorFunction> behavior_function);

	template <typename T>
	void setFunction(void){
		behavior_function = std::make_shared<T>();
	}

	~BehaviorComponent(void);
};

};

#endif
