#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "SystemManager.hpp"
#include "Components/Components.hpp"
#include "Scene.hpp"

namespace acc {

class PhysicsSystem : public System {
	public:
		PhysicsSystem(Scene *scene);
		void update(ComponentManager *component_manager);

	private:
		void updateTileset(ComponentManager *component_manager);
		bool solveCollisionBody(BodyComponent &body, TilesetComponent& tileset);
		void updateTransform(ComponentManager *component_manager, Entity entity);
		void callCollisionFunction(ComponentManager *component_manager, Entity entity, Entity other);

		Scene *scene;
};

};

#endif
