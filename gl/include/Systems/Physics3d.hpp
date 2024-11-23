#ifndef PHYSICS_3D_HPP
#define PHYSICS_3D_HPP

#include "SystemManager.hpp"
#include "Scene.hpp"

namespace acc {

class BrushBuilder;

class Physics3dSystem : public System {
	public:
		Physics3dSystem(Scene *scene);
		void update(ComponentManager *component_manager);

	private:
		void updatePhysics(ComponentManager *component_manager);
		void updateCollisions(ComponentManager *component_manager);
		void updateWorld(ComponentManager *component_manager);

		bool checkCollisionWorld(ComponentManager *component_manager, Entity entity, std::vector<BrushBuilder *> *found_intersections);
		bool checkCollisionMovBrush(ComponentManager *component_manager, Entity entity, bool solve);
		bool checkCollisionBody(ComponentManager *component_manager, Entity entity, std::vector<Entity> *found_intersections);
		void updateTransform(ComponentManager *component_manager, Entity entity);
		void callCollisionFunction(ComponentManager *component_manager, Entity entity, Entity other);

		Scene *scene;
		std::vector<Entity> found_intersections;
		std::vector<BrushBuilder *> found_brushes;
};

};

#endif
