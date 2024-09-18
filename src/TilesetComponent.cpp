#include "Components/TilesetComponent.hpp"

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
	collision_layer = collision_layer | (1 << layer);
}

int TilesetComponent::getTile(int x, int y){
	if(x < 0 || y < 0 || x >= TILESET_SIZE || y >= TILESET_SIZE) return -1;
	return tileset_tiles[x + y * TILESET_SIZE];
}

void TilesetComponent::setTile(int x, int y, int id){
	if(x < 0 || y < 0 || x >= TILESET_SIZE || y >= TILESET_SIZE) return;

	tileset_tiles[x + y * TILESET_SIZE] = id;
}

};
