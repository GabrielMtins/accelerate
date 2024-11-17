#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

#include "BaseTexture.hpp"
#include "Vec3.hpp"
#include "Color.hpp"

namespace acc {

struct SpriteComponent {
	/**
	 * The position of the sprite.
	 * It should follow the position of TransformComponent.
	 */
	Vec3 position;
	/**
	 * The scaling of the texture.
	 * Default: (1.0f, 1.0f, 1.0f).
	 */
	Vec3 scale;
	/**
	 * The center of rotation of the sprite.
	 * Default: (0, 0, 0).
	 */
	Vec3 center;
	/**
	 * The angle of the sprite.
	 * Default: 0.0f.
	 */
	float angle;

	/**
	 * The texture of the sprite.
	 * Default: NULL.
	 */
	BaseTexture *texture;
	/**
	 * The id of the texture to be drawn.
	 */
	int id;
	/**
	 * The texture which the sprite will be drawn.
	 * Default: 0.
	 */
	int layer;

	/**
	 * Sets if the sprite should follow the camera.
	 * Default: true.
	 */
	bool follow_camera;

	/**
	 * Flips the texture horizontally.
	 * Default: false.
	 */
	bool flip_horizontal;
	/**
	 * Flips the texture vertically.
	 * Default: false.
	 */
	bool flip_vertical;

	SpriteComponent(void);
	SpriteComponent(Resource *texture);
	SpriteComponent(BaseTexture *texture);
};

};

#endif
