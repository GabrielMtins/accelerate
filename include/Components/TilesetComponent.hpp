#ifndef TILESET_COMPONENT_HPP
#define TILESET_COMPONENT_HPP

#include <array>

#include "Texture.hpp"

#define DEFAULT_TILESET_SIZE 256

namespace acc {

struct TilesetComponent {
	/**
	 * The tileset texture of the component
	 */
	Texture *tileset_texture;
	/**
	 * The vector that contains the tiles of the level.
	 */
	std::vector<int> tileset_tiles;
	/**
	 * The tileset width. It's a squared grid.
	 */
	int tileset_width;
	/**
	 * The tileset height. It's a squared grid.
	 */
	int tileset_height;
	/**
	 * The width of the tile.
	 */
	int width;
	/**
	 * The height of the tile.
	 */
	int height;
	/**
	 * A bitmask which contains which layers the component is in.
	 */
	uint32_t collision_layer;
	/**
	 * The layer which the components gets drawn.
	 */
	int draw_layer;

	TilesetComponent(void);
	TilesetComponent(Texture *tileset_texture, int tileset_width, int tileset_height, int width, int height);
	TilesetComponent(Resource *tileset_texture, int tileset_width, int tileset_height, int width, int height);

	void setCollisionLayer(int layer);
	int getTile(int x, int y);
	void setTile(int x, int y, int id);

	bool intersectsLine(Vec3 origin, Vec3 dir, Vec3 *return_intersection);
};
	
};

#endif
