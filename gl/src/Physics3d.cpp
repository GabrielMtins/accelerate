#include "Systems/Physics3d.hpp"
#include "Components/Body3dComponent.hpp"
#include "Components/World3dComponent.hpp"
#include "Components/MovBrush3dComponent.hpp"
#include "Components/Components.hpp"

namespace acc {

Physics3dSystem::Physics3dSystem(Scene *scene){
	this->scene = scene;
}

void Physics3dSystem::update(ComponentManager *component_manager){
	updatePhysics(component_manager);
	updateCollisions(component_manager);
	updateWorld(component_manager);
}

void Physics3dSystem::updatePhysics(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<Body3dComponent>())
		return;

	auto arr = component_manager->getComponentArray<Body3dComponent>();

	float dt = scene->getGame()->getContext()->getDeltaTime();

	for(size_t i = 0; i < arr->getSize(); i++){
		Entity entity = arr->indexToEntity(i);

		auto& physics = arr->atIndex(i);

		physics.position += physics.velocity * dt;
		physics.velocity += physics.gravity * dt;

		updateTransform(component_manager, entity);
	}
}

void Physics3dSystem::updateCollisions(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<Body3dComponent>())
		return;

	auto arr = component_manager->getComponentArray<Body3dComponent>();

	found_intersections.reserve(arr->getSize());

	for(size_t i = 0; i < arr->getSize(); i++){
		found_intersections.clear();
		Entity entity = arr->indexToEntity(i);
		auto& phy1 = arr->atIndex(i);

		if(phy1.collision_mask == 0 && phy1.collision_trigger == 0)
			continue;

		if(checkCollisionBody(component_manager, entity, &found_intersections)){
			for(Entity other : found_intersections){
				auto& phy2 = arr->getComponent(other);

				Vec3 old_position = phy1.position;

				phy1.solveCollision(phy2);

				if(checkCollisionBody(component_manager, entity, NULL) || checkCollisionWorld(component_manager, entity, NULL)){
					phy1.position = old_position;
					old_position = phy2.position;

					phy2.solveCollision(phy1);

					if(checkCollisionBody(component_manager, other, NULL) || checkCollisionWorld(component_manager, other, NULL)){
						phy2.position = old_position;
					}
				}

				callCollisionFunction(component_manager, entity, other);

				updateTransform(component_manager, entity);
				updateTransform(component_manager, other);
			}
		}
	}
}

void Physics3dSystem::updateWorld(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<Body3dComponent>())
		return;

	auto arr = component_manager->getComponentArray<Body3dComponent>();
	found_brushes.clear();

	for(size_t i = 0; i < arr->getSize(); i++){
		Entity entity = arr->indexToEntity(i);
		auto& body = arr->atIndex(i);

		checkCollisionWorld(component_manager, entity, &found_brushes);

		for(BrushBuilder *brush : found_brushes){
			body.solveCollision(brush);
		}

		checkCollisionMovBrush(component_manager, entity, true);

		updateTransform(component_manager, entity);
	}
}

bool Physics3dSystem::checkCollisionWorld(ComponentManager *component_manager, Entity entity, std::vector<BrushBuilder *> *found_intersections){
	bool found_collision = false;

	if(!component_manager->hasComponentArray<World3dComponent>())
		return found_collision;

	auto arr = component_manager->getComponentArray<World3dComponent>();
	auto& body = component_manager->getComponent<Body3dComponent>(entity);

	for(size_t i = 0; i < arr->getSize(); i++){
		World3dComponent& world = arr->atIndex(i);

		if(((body.collision_trigger & world.collision_layer) == 0) && ((body.collision_mask & world.collision_layer) == 0))
			continue;

		for(BrushBuilder *brush : world.brushes){
			Vec3 delta;

			if(body.checkCollision(brush)){
				found_collision = true;

				if(found_intersections != NULL)
					found_intersections->push_back(brush);
			}
		}
	}

	return found_collision;
}

bool Physics3dSystem::checkCollisionMovBrush(ComponentManager *component_manager, Entity entity, bool solve){
	bool found_collision = false;

	if(!component_manager->hasComponentArray<MovBrush3dComponent>())
		return found_collision;

	auto arr = component_manager->getComponentArray<MovBrush3dComponent>();
	auto& body = component_manager->getComponent<Body3dComponent>(entity);
	Vec3 old_pos = body.position;

	for(size_t i = 0; i < arr->getSize(); i++){
		MovBrush3dComponent& brush = arr->atIndex(i);
		Entity current_entity = arr->indexToEntity(i);

		if(component_manager->hasComponent<TransformComponent>(current_entity)){
			auto& transform = component_manager->getComponent<TransformComponent>(current_entity);
			brush.position = transform.position;
		}

		if(((body.collision_trigger & brush.collision_layer) == 0) && ((body.collision_mask & brush.collision_layer) == 0))
			continue;

		if(solve){
			old_pos = body.position;

			found_collision = found_collision || body.solveCollision(brush.brush, brush.position);

			if(checkCollisionWorld(component_manager, entity, NULL)){
				body.position = old_pos;
			}
		}
		else{
			found_collision = found_collision | body.checkCollision(brush.brush, brush.position);
		}
	}

	return found_collision;
}

bool Physics3dSystem::checkCollisionBody(ComponentManager *component_manager, Entity entity, std::vector<Entity> *found_intersections){
	bool found_collision = false;

	if(!component_manager->hasComponentArray<Body3dComponent>())
		return found_collision;

	auto arr = component_manager->getComponentArray<Body3dComponent>();
	auto& phy1 = arr->getComponent(entity);

	for(size_t i = 0; i < arr->getSize(); i++){
		Entity other = arr->indexToEntity(i);

		if(other == entity)
			continue;

		auto& phy2 = arr->atIndex(i);

		if(phy1.checkCollision(phy2)){
			//printf("found\n");

			if(found_intersections != NULL)
				found_intersections->push_back(other);

			found_collision = true;
		}
		else{
			//printf("%u %u\n", phy1.collision_mask, phy1.collision_trigger);
			//printf("not found\n");
		}

	}

	return found_collision;
}

void Physics3dSystem::updateTransform(ComponentManager *component_manager, Entity entity){
	if(!component_manager->hasComponent<TransformComponent>(entity))
		return;

	auto& physics = component_manager->getComponent<Body3dComponent>(entity);
	auto& transform = component_manager->getComponent<TransformComponent>(entity);
	transform.position = physics.position - physics.offset_from_transform;
}

void Physics3dSystem::callCollisionFunction(ComponentManager *component_manager, Entity entity, Entity other){
	if(!component_manager->hasComponent<BehaviorComponent>(entity))
		return;

	auto& behave = component_manager->getComponent<BehaviorComponent>(entity);

	if(behave.behavior_function->getScene() != NULL)
		behave.behavior_function->onCollision(other);
}

};
