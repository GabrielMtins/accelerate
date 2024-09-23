#include "Systems/Render2d.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"

#include <iostream>
#include <algorithm>
#include <cmath>

namespace acc {

Render2dSystem::Render2dSystem(Context *context, Vec3 *camera_position){
	this->context = context;
	this->camera_position = camera_position;
}

void Render2dSystem::update(ComponentManager *component_manager){
	updateText(component_manager);
	updateTileset(component_manager);
	updateSprites(component_manager);
}

void Render2dSystem::updateText(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<TextComponent>())
		return;

	auto arr = component_manager->getComponentArray<TextComponent>();

	for(size_t i = 0; i < arr->getSize(); i++){
		Entity entity = arr->indexToEntity(i);

		if(!component_manager->hasComponent<SpriteComponent>(entity))
			continue;

		auto& text_component = arr->atIndex(i);
		auto& sprite_component = component_manager->getComponent<SpriteComponent>(entity);

		if(text_component.updateTexture(context)){
			sprite_component.texture = text_component.texture.get();
		}
	}
}

void Render2dSystem::updateSprites(ComponentManager *component_manager){
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

		/* if the sprite isn't on screen, we can just skip it */
		if(!isSpriteOnCamera(current_sprite)) continue;

		render_array.push_back(&current_sprite);
	}

	std::sort(render_array.begin(), render_array.end(), Render2dSystem::customTextureLess);

	for(auto& i : render_array){
		if(i->texture == NULL) continue;

		Vec3 sprite_position = i->position;

		if(i->follow_camera) sprite_position -= *camera_position;

		i->texture->renderCell(
				context,
				floor(sprite_position.x),
				floor(sprite_position.y),
				i->id
				);
	}
}

void Render2dSystem::updateTileset(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<TilesetComponent>())
		return;

	auto arr = component_manager->getComponentArray<TilesetComponent>();

	for(size_t i = 0; i < arr->getSize(); i++){
		auto& current_tileset = arr->atIndex(i);
		renderTilesetComponent(current_tileset);
	}
}

void Render2dSystem::renderTilesetComponent(TilesetComponent& tileset){
	int min_x = (int) (camera_position->x / tileset.width) - 2;
	int min_y = (int) (camera_position->y / tileset.height) - 2;
	int max_x = min_x + context->getWidth() / tileset.width + 2;
	int max_y = min_y + context->getHeight() / tileset.height + 2;

	for(int i = min_x; i < max_x; i++){
		for(int j = min_y; j < max_y; j++){
			Vec3 tile_pos = Vec3(i * tileset.width, j * tileset.height, 0);
			int tile_id = tileset.getTile(i, j);

			if(tile_id == -1) continue;


			tileset.tileset_texture->renderCell(
					context,
					tile_pos.x - camera_position->x,
					tile_pos.y - camera_position->y,
					tile_id
					);
		}
	}
}

bool Render2dSystem::isSpriteOnCamera(const SpriteComponent &sprite){
	Vec3 pos_min, pos_max;

	pos_min = sprite.position;

	if(sprite.follow_camera)
		pos_min -= *camera_position;

	pos_max = pos_min + Vec3(sprite.texture->getCellWidth(), sprite.texture->getCellHeight());

	if(pos_max.x < 0) return false;
	if(pos_min.x > context->getWidth()) return false;

	if(pos_max.y < 0) return false;
	if(pos_min.y > context->getHeight()) return false;

	return true;
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
