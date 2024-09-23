#ifndef BODY_COMPONENT_HPP
#define BODY_COMPONENT_HPP

#include "global.hpp"
#include "Vec3.hpp"

namespace acc {

struct BodyComponent {
	Vec3 position;
	Vec3 velocity;
	Vec3 offset_from_transform;
	Vec3 size;
	Vec3 gravity;

	bool is_trigger;

	uint32_t collision_layer; /* which layers the body is in */
	uint32_t collision_mask; /* which layers the body can collide with */

	BodyComponent(void);

	void setOnCollisionLayer(size_t layer, bool is_on);
	void setOnCollisionMask(size_t layer, bool is_on);

	bool checkCollision(BodyComponent other);
	bool checkCollision(Vec3 point);
	void solveCollision(BodyComponent other);
	bool checkLineIntersection(Vec3 start, Vec3 direction, Vec3 *return_intersection);
};

};

#endif
