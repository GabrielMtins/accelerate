#include "Systems/Physics.hpp"
#include "Game.hpp"

#include "Components/Components.hpp"

namespace acc {
		
PhysicsSystem::PhysicsSystem(Scene *scene){
	this->scene = scene;
}

void PhysicsSystem::update(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<BodyComponent>())
		return;

	auto arr = component_manager->getComponentArray<BodyComponent>();

	float dt = scene->getGame()->getContext()->getDeltaTime();

	for(size_t i = 0; i < arr->getSize(); i++){
		auto& physics = arr->atIndex(i);

		physics.velocity += physics.gravity * dt;
		physics.position += physics.velocity * dt;
	}

	for(size_t i = 0; i < arr->getSize(); i++){
		Entity entity = arr->indexToEntity(i);

		/* check collision */
		for(size_t j = 0; j < arr->getSize(); j++){
			if(i == j) continue;

			Entity other = arr->indexToEntity(j);

			auto& phy1 = arr->atIndex(i);
			auto& phy2 = arr->atIndex(j);

			if(phy1.checkCollision(phy2)){
				if(!phy1.is_trigger)
					phy1.solveCollision(phy2);

				callCollisionFunction(component_manager, entity, other);
			}
		}

		updateTransform(component_manager, entity);
	}
}

void PhysicsSystem::updateTransform(ComponentManager *component_manager, Entity entity){
	if(!component_manager->hasComponent<TransformComponent>(entity))
		return;

	auto& physics = component_manager->getComponent<BodyComponent>(entity);
	auto& transform = component_manager->getComponent<TransformComponent>(entity);
	transform.position = physics.position - physics.offset_from_transform;
}

void PhysicsSystem::callCollisionFunction(ComponentManager *component_manager, Entity entity, Entity other){
	if(!component_manager->hasComponent<BehaviorComponent>(entity))
		return;

	auto& behave = component_manager->getComponent<BehaviorComponent>(entity);
	behave.behavior_function->onCollision(other);
}

};
