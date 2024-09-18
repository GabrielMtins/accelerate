#include "Systems/Render2d.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"

#include <iostream>
#include <algorithm>

namespace acc {

Render2dSystem::Render2dSystem(Context *context, Vec3 *camera_position){
	this->context = context;
	this->camera_position = camera_position;
}

void Render2dSystem::update(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<SpriteComponent>())
		return;

	auto arr = component_manager->getComponentArray<SpriteComponent>();
	std::vector<SpriteComponent *> render_array;

	for(size_t i = 0; i < arr->getSize(); i++){
		Entity entity = arr->indexToEntity(i);

		auto& current_sprite = arr->atIndex(i);

		/* ignore if it doesn't have a transform component */
		if(!component_manager->hasComponent<TransformComponent>(entity))
			continue;

		auto& current_transform = component_manager->getComponent<TransformComponent>(entity);

		current_sprite.position = current_transform.position;

		render_array.push_back(&current_sprite);
	}

	std::sort(render_array.begin(), render_array.end(), Render2dSystem::customTextureLess);

	for(auto& i : render_array){
		if(i->texture == NULL) continue;

		i->texture->renderCell(
				context,
				i->position.x, i->position.y,
				i->id
				);
	}
}

bool Render2dSystem::customTextureLess(SpriteComponent *a, SpriteComponent *b){
	if(a->layer == b->layer){
		return
			a->position.y + a->texture->getCellHeight() <
			b->position.y + b->texture->getCellHeight();
	}
	
	return a->layer < b->layer;
}

};
