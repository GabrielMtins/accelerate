#ifndef BODY_COMPONENT_HPP
#define BODY_COMPONENT_HPP

#include "global.hpp"
#include "Vec3.hpp"

namespace acc {

struct BodyComponent {
	/**
	 * The position of the body component.
	 * Default: (0, 0, 0).
	 */
	Vec3 position;
	/**
	 * The current velocity of the body component.
	 * Default: (0, 0, 0).
	 */
	Vec3 velocity;
	/**
	 * The offset from transform. The transform position can be
	 * computed as:
	 * transform.position = body.position - body.offset_from_transform;
	 * Default: (0, 0, 0).
	 */
	Vec3 offset_from_transform;
	/**
	 * The hitbox size of the body. It's always an axis aligned bounding box.
	 * Default: (0, 0, 0).
	 */
	Vec3 size;
	/**
	 * The gravity applied to the body.
	 * Default: (0, 0, 0).
	 */
	Vec3 gravity;

	/**
	 * A bitmask variable containing the layers which the body is in.
	 */
	uint32_t collision_layer;
	/**
	 * A bitmask variable containg the layers which the body can collide with.
	 */
	uint32_t collision_mask;

	/**
	 * A bitmask variable containg the layers which the body will trigger if it collides with.
	 */
	uint32_t collision_trigger;

	BodyComponent(void);

	/**
	 * Sets if the body is at the layer from the argument or not.
	 */
	void setOnCollisionLayer(size_t layer, bool is_on);
	/**
	 * Sets if the body can collide with the layer from the argument or not.
	 */
	void setOnCollisionMask(size_t layer, bool is_on);

	/**
	 * Sets if the body wil trigger with the layer from the argument or not.
	 */
	void setOnCollisionTrigger(size_t layer, bool is_on);

	/**
	 * Returns true if it collides with the "other" body.
	 */
	bool checkCollision(const BodyComponent& other);
	/**
	 * Returns true if the point is inside the body.
	 */
	bool checkCollision(const Vec3& point);
	/**
	 * Solves the collision between the two bodies.
	 */
	void solveCollision(const BodyComponent& other);
	/**
	 * Check intersections between the body and a given line.
	 * Returns true if there's a solution such that:
	 * *return_intersection = start + direction * u, with u >= 0.0f
	 */
	bool checkLineIntersection(Vec3 start, Vec3 direction, Vec3 *return_intersection);
};

};

#endif
