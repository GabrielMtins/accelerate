#ifndef SECTOR_HPP
#define SECTOR_HPP

#include "Vec3.hpp"
#include <vector>

namespace acc {

struct TextureConfiguration {
	int id;
	Vec3 offset;
	Vec3 scaling;

	TextureConfiguration(void);
};

struct Wall {
	Vec3 a, b;
	int portal;

	TextureConfiguration texture;
};

struct Sector {
	std::vector<Wall> walls;

	float floor_height;
	TextureConfiguration floor_texture;

	float celling_height;
	TextureConfiguration celling_texture;
};

};

#endif
