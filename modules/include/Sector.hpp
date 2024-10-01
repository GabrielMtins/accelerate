#ifndef SECTOR_HPP
#define SECTOR_HPP

#include "Vec3.hpp"
#include <vector>

namespace acc {

struct TextureConfiguration {
	int texture_id;
	Vec3 texture_offset;
	Vec3 texture_scaling;
};

struct Wall {
	Vec3 a, b;
	float size;
	int portal;

	TextureConfiguration texture_configuration;
};

struct Sector {
	std::vector<Wall> walls;

	float floor_height;
	TextureConfiguration floor_texture_configuration;

	float celling_height;
	TextureConfiguration celling_texture_configuration;
};

};

#endif
