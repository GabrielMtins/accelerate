#ifndef TILESET_COMPONENT_HPP
#define TILESET_COMPONENT_HPP

#include <array>

#include "Texture.hpp"

#define DEFAULT_TILESET_SIZE 256

namespace acc {

struct TilesetComponent {
	Texture *tileset_texture;
	std::vector<int> tileset_tiles;
	int tileset_size;
	int width, height;
	uint32_t collision_layer;
	int draw_layer;

	TilesetComponent(void);
	TilesetComponent(Texture *tileset_texture, int tileset_size, int width, int height);

	void setCollisionLayer(int layer);
	int getTile(int x, int y);
	void setTile(int x, int y, int id);

	bool intersectsLine(Vec3 origin, Vec3 dir, Vec3 *return_intersection);
};
	
};

#endif
