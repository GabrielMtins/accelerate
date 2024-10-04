#include "Systems/Render2d.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"

#include <iostream>
#include <algorithm>
#include <cmath>

namespace acc {

RenderData::RenderData(SpriteComponent *sprite){
	data.sprite = sprite;

	type = RENDER_DATA_SPRITE;
	layer = sprite->layer;

	if(sprite->texture != NULL)
		y = sprite->position.y + sprite->texture->getCellHeight();
	else
		y = -1;
}

RenderData::RenderData(DrawRectComponent *rect){
	data.rect = rect;

	type = RENDER_DATA_RECT;
	layer = rect->layer;
	y = rect->y + rect->h;
}

RenderData::RenderData(TilesetComponent *tileset){
	data.tileset = tileset;

	type = RENDER_DATA_TILESET;
	layer = tileset->draw_layer;
	y = -1;
}

Render2dSystem::Render2dSystem(Context *context, Vec3 *camera_position){
	this->context = context;
	this->camera_position = camera_position;
}

void Render2dSystem::update(ComponentManager *component_manager){
	/* render the tilesets */
	updateTileset(component_manager);

	/* update sprites and etc */
	updateText(component_manager);
	updateSprites(component_manager);
	updateDrawRects(component_manager);

	/* render everything */
	renderAll();
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

	auto sprite_array = component_manager->getComponentArray<SpriteComponent>();

	for(size_t i = 0; i < sprite_array->getSize(); i++){
		Entity entity = sprite_array->indexToEntity(i);

		auto& current_sprite = sprite_array->atIndex(i);

		/* ignore if it doesn't have a transform component */
		if(!component_manager->hasComponent<TransformComponent>(entity))
			continue;

		/* ignore if sprite doesn't have a texture */
		if(current_sprite.texture == NULL)
			continue;

		auto& current_transform = component_manager->getComponent<TransformComponent>(entity);

		current_sprite.position = current_transform.position;

		/* if the sprite isn't on screen, we can just skip it */
		if(!isSpriteOnCamera(current_sprite)) continue;

		render_array.push_back(RenderData(&current_sprite));
	}
}

void Render2dSystem::updateDrawRects(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<DrawRectComponent>())
		return;

	auto rect_array = component_manager->getComponentArray<DrawRectComponent>();

	for(size_t i = 0; i < rect_array->getSize(); i++){
		Entity entity = rect_array->indexToEntity(i);
		auto& current_rect = rect_array->atIndex(i);

		if(!component_manager->hasComponent<TransformComponent>(entity))
			continue;

		auto& current_transform = component_manager->getComponent<TransformComponent>(entity);
		current_rect.x = current_transform.position.x;
		current_rect.y = current_transform.position.y;

		/* if the sprite isn't on screen, we can just skip it */
		if(!isRectOnCamera(current_rect)) continue;

		render_array.push_back(RenderData(&current_rect));
	}
}

void Render2dSystem::updateTileset(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<TilesetComponent>())
		return;

	auto arr = component_manager->getComponentArray<TilesetComponent>();

	for(size_t i = 0; i < arr->getSize(); i++){
		auto& current_tileset = arr->atIndex(i);
		render_array.push_back(RenderData(&current_tileset));
	}
}

void Render2dSystem::renderTilesetComponent(TilesetComponent* tileset){
	int min_x = (int) (camera_position->x / tileset->width) - 2;
	int min_y = (int) (camera_position->y / tileset->height) - 2;
	int max_x = min_x + context->getWidth() / tileset->width + 2;
	int max_y = min_y + context->getHeight() / tileset->height + 2;

	for(int i = min_x; i < max_x; i++){
		for(int j = min_y; j < max_y; j++){
			Vec3 tile_pos = Vec3(i * tileset->width, j * tileset->height, 0);
			int tile_id = tileset->getTile(i, j);

			if(tile_id == -1) continue;


			tileset->tileset_texture->renderCell(
					context,
					tile_pos.x - camera_position->x,
					tile_pos.y - camera_position->y,
					tile_id
					);
		}
	}
}

void Render2dSystem::renderAll(void){
	std::sort(render_array.begin(), render_array.end(), Render2dSystem::customTextureLess);

	for(auto& i : render_array){
		if(i.type == RENDER_DATA_SPRITE) {
			SpriteComponent* sprite = i.data.sprite;
			Vec3 sprite_position = sprite->position;

			if(sprite->follow_camera) sprite_position -= *camera_position;
			if(sprite->texture == NULL) continue;

			sprite->texture->renderCellEx(
					context,
					floor(sprite_position.x),
					floor(sprite_position.y),
					sprite->id,
					sprite->scale.x,
					sprite->scale.y,
					sprite->center.x,
					sprite->center.y,
					sprite->angle
					);
		}
		else if(i.type == RENDER_DATA_RECT) {
			SDL_Renderer *renderer = context->getRenderer();
			DrawRectComponent *rect = i.data.rect;

			SDL_SetRenderDrawColor(renderer, rect->color.r, rect->color.g, rect->color.b, rect->color.a);

			SDL_Rect sdl_rect = {
				(int) (rect->x - camera_position->x),
				(int) (rect->y - camera_position->y),
				rect->w,
				rect->h
			};

			SDL_RenderFillRect(renderer, &sdl_rect);
		}
		else if(i.type == RENDER_DATA_TILESET){
			renderTilesetComponent(i.data.tileset);
		}
	}

	render_array.clear();
}

bool Render2dSystem::isSpriteOnCamera(const SpriteComponent &sprite){
	if(!sprite.follow_camera) return true;
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

bool Render2dSystem::isRectOnCamera(const DrawRectComponent &rect){
	int actual_x, actual_y;

	actual_x = rect.x;
	actual_y = rect.y;

	if(rect.follow_camera){
		actual_x -= camera_position->x;
		actual_y -= camera_position->y;
	}

	if(actual_x > context->getWidth()) return false;
	if(actual_y > context->getHeight()) return false;
	if(actual_x + rect.w < 0) return false;
	if(actual_y + rect.h < 0) return false;

	return true;
}

bool Render2dSystem::customTextureLess(const RenderData& a, const RenderData& b){
	if(a.layer == b.layer){
		return a.y < b.y;
	}
	
	return a.layer < b.layer;
}

};
