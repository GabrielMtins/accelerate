#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

#include "Texture.hpp"
#include "Vec3.hpp"
#include "Color.hpp"

namespace acc {

struct SpriteComponent {
	Vec3 position;
	Vec3 scale;
	Vec3 center;
	float angle;

	Texture *texture;
	int id;
	int layer;

	bool follow_camera;

	SpriteComponent(void);
	SpriteComponent(Resource *texture);
	SpriteComponent(Texture *texture);
};

};

#endif
