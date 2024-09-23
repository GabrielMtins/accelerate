#include "Components/TilesetComponent.hpp"

#include <cmath>

namespace acc {

TilesetComponent::TilesetComponent(void){
	tileset_texture = NULL;
	width = height = 0;

	for(size_t i = 0; i < TILESET_SIZE * TILESET_SIZE; i++)
		tileset_tiles[i] = -1;

	collision_layer = 0;
}

TilesetComponent::TilesetComponent(Texture *tileset_texture, int width, int height){
	this->tileset_texture = tileset_texture;
	this->width = width;
	this->height = height;

	for(size_t i = 0; i < TILESET_SIZE * TILESET_SIZE; i++)
		tileset_tiles[i] = -1;

	collision_layer = 0;
}

void TilesetComponent::setCollisionLayer(int layer){
	collision_layer = collision_layer | (1 << (layer - 1));
}

int TilesetComponent::getTile(int x, int y){
	if(x < 0 || y < 0 || x >= TILESET_SIZE || y >= TILESET_SIZE) return -1;
	return tileset_tiles[x + y * TILESET_SIZE];
}

void TilesetComponent::setTile(int x, int y, int id){
	if(x < 0 || y < 0 || x >= TILESET_SIZE || y >= TILESET_SIZE) return;

	tileset_tiles[x + y * TILESET_SIZE] = id;
}

bool TilesetComponent::intersectsLine(Vec3 origin, Vec3 dir, Vec3 *return_intersection){
	bool found_intersection = false;

	Vec3 new_origin = Vec3(floorf(origin.x / width), floorf(origin.y / height));
	Vec3 new_dir = dir / fmaxf(fabsf(dir.x), fabsf(dir.y));

	while(!found_intersection){
		if(new_origin.x < 0 || new_origin.y < 0 || new_origin.x >= TILESET_SIZE || new_origin.y >= TILESET_SIZE){
			found_intersection = false;
			break;
		}

		if(getTile(new_origin.x, new_origin.y) != -1){
			found_intersection = true;
			break;
		}

		new_origin += new_dir;
	}

	if(found_intersection){
		*return_intersection = (Vec3(floorf(new_origin.x), floorf(new_origin.y)) + Vec3(0.5, 0.5)) * Vec3(width, height);
	}

	return found_intersection;
}

};
