#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

#include "Texture.hpp"
#include "Vec3.hpp"

namespace acc {

struct SpriteComponent {
	Vec3 position;
	Texture *texture;
	int id;
	int layer;

	SpriteComponent(void);
	SpriteComponent(Texture *texture);
};

};

#endif
