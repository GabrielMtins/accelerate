#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "SystemManager.hpp"
#include "Scene.hpp"

namespace acc {

class PhysicsSystem : public System {
	public:
		PhysicsSystem(Scene *scene);
		void update(ComponentManager *component_manager);

	private:
		void updateTransform(ComponentManager *component_manager, Entity entity);
		void callCollisionFunction(ComponentManager *component_manager, Entity entity, Entity other);

		Scene *scene;
};

};

#endif
