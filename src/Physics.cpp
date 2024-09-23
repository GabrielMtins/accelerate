#include <algorithm>

#include "Systems/Physics.hpp"
#include "Game.hpp"

#include "Components/Components.hpp"

namespace acc {
		
PhysicsSystem::PhysicsSystem(Scene *scene){
	this->scene = scene;
}

void PhysicsSystem::update(ComponentManager *component_manager){
	updatePhysics(component_manager);
	updateCollisions(component_manager);
	updateTileset(component_manager);
}

void PhysicsSystem::updateCollisions(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<BodyComponent>())
		return;

	auto arr = component_manager->getComponentArray<BodyComponent>();

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
	}
}

void PhysicsSystem::updatePhysics(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<BodyComponent>())
		return;

	auto arr = component_manager->getComponentArray<BodyComponent>();

	float dt = scene->getGame()->getContext()->getDeltaTime();

	for(size_t i = 0; i < arr->getSize(); i++){
		Entity entity = arr->indexToEntity(i);

		auto& physics = arr->atIndex(i);

		physics.position += physics.velocity * dt;
		physics.velocity += physics.gravity * dt;

		updateTransform(component_manager, entity);
	}
}

void PhysicsSystem::updateTileset(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<BodyComponent>())
		return;

	if(!component_manager->hasComponentArray<TilesetComponent>())
		return;

	auto body_array = component_manager->getComponentArray<BodyComponent>();
	auto tile_array = component_manager->getComponentArray<TilesetComponent>();

	for(size_t i = 0; i < body_array->getSize(); i++){
		Entity entity = body_array->indexToEntity(i);

		for(size_t j = 0; j < tile_array->getSize(); j++){
			auto& body = body_array->atIndex(i);
			auto& tileset = tile_array->atIndex(j);
			bool found_collision = false;

			if(solveCollisionBody(body, tileset)){
				found_collision = true;
			}

			if(found_collision){
				callCollisionFunction(component_manager, entity, tile_array->indexToEntity(j));
				updateTransform(component_manager, entity);
			}
		}
	}
}

bool PhysicsSystem::solveCollisionBody(BodyComponent &body, TilesetComponent& tileset){
	if((body.collision_mask & tileset.collision_layer) == 0) return false;

	bool found_collision = false;

	BodyComponent old_body;
	std::vector<BodyComponent> final_body;

	old_body = body;

	Vec3 start = Vec3(
			(int) body.position.x / tileset.width - 1,
			(int) body.position.y / tileset.height - 1,
			0
			);

	Vec3 size = Vec3(
			(int) body.size.x / tileset.width + 2,
			(int) body.size.y / tileset.height + 2,
			0
			);

	for(int i = start.x; i < start.x + size.x; i++){
		for(int j = start.y; j < start.y + size.y; j++){
			if(tileset.getTile(i, j) == -1) continue;

			BodyComponent tile;

			tile.position = Vec3(i, j, 0) * Vec3(tileset.width, tileset.height, 0);
			tile.size = Vec3(tileset.width, tileset.height, 0);
			tile.collision_layer = tileset.collision_layer;

			if(body.checkCollision(tile)){
				if(!body.is_trigger){
					body.solveCollision(tile);
					/*
					final_body.push_back(body);
					body = old_body;
					*/
				}

				found_collision = true;
			}
		}
	}

	/*
	if(found_collision && !body.is_trigger){
		std::sort(final_body.begin(), final_body.end(),
				[old_body](BodyComponent a, BodyComponent b){
					float da = (a.position - old_body.position).length();
					float db = (b.position - old_body.position).length();

					if(da == 0) return true;

					return da < db;
				}
				);

		body = final_body[0];
	}
	*/

	return found_collision;
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
