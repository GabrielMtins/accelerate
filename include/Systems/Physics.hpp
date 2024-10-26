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
		
		static bool raycast(ComponentManager *component_manager, Vec3 origin, Vec3 direction, uint32_t layer_mask, Entity *return_entity, Vec3 *return_intersection);


	private:
		static bool raycastBody(ComponentManager *component_manager, Vec3 origin, Vec3 direction, uint32_t layer_mask, Entity *return_entity, Vec3 *return_intersection);
		static bool raycastTileset(ComponentManager *component_manager, Vec3 origin, Vec3 direction, uint32_t layer_mask, Entity *return_entity, Vec3 *return_intersection);

		void updateCollisions(ComponentManager *component_manager);

		bool checkCollisionBody(ComponentManager *component_manager, Entity entity, std::vector<Entity> *found_intersections);
		bool checkCollisionForTileset(ComponentManager *component_manager, BodyComponent& body);

		void updatePhysics(ComponentManager *component_manager);
		void updateTileset(ComponentManager *component_manager);
		bool solveCollisionBody(BodyComponent &body, TilesetComponent& tileset);
		void updateTransform(ComponentManager *component_manager, Entity entity);
		void callCollisionFunction(ComponentManager *component_manager, Entity entity, Entity other);

		Scene *scene;
		std::vector<Entity> found_intersections;
};

};

#endif
