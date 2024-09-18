#include "Components/TilesetComponent.hpp"

namespace acc {

TilesetComponent::TilesetComponent(void){
	tileset_texture = NULL;
	width = height = 0;
}

TilesetComponent::TilesetComponent(Texture *tileset_texture, int width, int height){
	this->tileset_texture = tileset_texture;
	this->width = width;
	this->height = height;
}

int TilesetComponent::getTile(int x, int y){
	return tileset_tiles[x + y * TILESET_SIZE];
}

void TilesetComponent::setTile(int x, int y, int id){
	tileset_tiles[x + y * TILESET_SIZE] = id;
}

};
