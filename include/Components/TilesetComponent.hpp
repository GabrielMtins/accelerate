#ifndef TILESET_COMPONENT_HPP
#define TILESET_COMPONENT_HPP

#include <array>

#include "Texture.hpp"

#define TILESET_SIZE 256

namespace acc {

struct TilesetComponent {
	Texture *tileset_texture;
	std::array<int, TILESET_SIZE * TILESET_SIZE> tileset_tiles;
	int width, height;
	uint32_t collision_layer;

	TilesetComponent(void);
	TilesetComponent(Texture *tileset_texture, int width, int height);

	void setCollisionLayer(int layer);
	int getTile(int x, int y);
	void setTile(int x, int y, int id);

	bool intersectsLine(Vec3 origin, Vec3 dir, Vec3 *return_intersection);
};
	
};

#endif
