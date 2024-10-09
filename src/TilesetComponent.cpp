#include "Components/TilesetComponent.hpp"

#include <cmath>

namespace acc {

TilesetComponent::TilesetComponent(void){
	tileset_texture = NULL;
	width = height = 0;
	tileset_width = DEFAULT_TILESET_SIZE;
	tileset_height = DEFAULT_TILESET_SIZE;
	draw_layer = 0;

	tileset_tiles.resize(tileset_width * tileset_height);

	for(auto& i : tileset_tiles)
		i = -1;

	collision_layer = 0;
}

TilesetComponent::TilesetComponent(Texture *tileset_texture, int tileset_width, int tileset_height, int width, int height){
	this->tileset_texture = tileset_texture;
	this->width = width;
	this->height = height;
	this->tileset_width = tileset_width;
	this->tileset_height = tileset_height;
	draw_layer = 0;

	tileset_tiles.resize(tileset_width * tileset_height);

	for(auto& i : tileset_tiles)
		i = -1;

	collision_layer = 0;
}

TilesetComponent::TilesetComponent(Resource *tileset_texture, int tileset_width, int tileset_height, int width, int height){
	this->tileset_texture = (Texture *) tileset_texture;
	this->width = width;
	this->height = height;
	this->tileset_width = tileset_width;
	this->tileset_height = tileset_height;
	draw_layer = 0;

	tileset_tiles.resize(tileset_width * tileset_height);

	for(auto& i : tileset_tiles)
		i = -1;

	collision_layer = 0;
}

void TilesetComponent::setCollisionLayer(int layer){
	collision_layer = collision_layer | (1 << (layer - 1));
}

int TilesetComponent::getTile(int x, int y){
	if(x < 0 || y < 0 || x >= tileset_width || y >= tileset_height) return -1;
	return tileset_tiles[x + y * tileset_width];
}

void TilesetComponent::setTile(int x, int y, int id){
	if(x < 0 || y < 0 || x >= tileset_width || y >= tileset_height) return;

	tileset_tiles[x + y * tileset_width] = id;
}

bool TilesetComponent::intersectsLine(Vec3 origin, Vec3 dir, Vec3 *return_intersection){
	bool found_intersection = false;

	Vec3 new_origin = Vec3(floorf(origin.x / width), floorf(origin.y / height));
	Vec3 new_dir = dir / fmaxf(fabsf(dir.x), fabsf(dir.y));

	while(!found_intersection){
		if(new_origin.x < 0 || new_origin.y < 0 || new_origin.x >= tileset_width || new_origin.y >= tileset_height){
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
