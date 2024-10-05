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
	 * Tells if a body is a trigger or not. If the body is a trigger, its collision
	 * will not be solved, but it'll try to call the BehaviorFunction::onCollision
	 * method if the entity has one.
	 */
	bool is_trigger;

	/**
	 * A bitmask variable containing the layers which the body is in.
	 */
	uint32_t collision_layer;
	/**
	 * A bitmask variable containg the layers which the body can collide with.
	 */
	uint32_t collision_mask;

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
	 * Returns true if it collides with the "other" body.
	 */
	bool checkCollision(BodyComponent other);
	/**
	 * Returns true if the point is inside the body.
	 */
	bool checkCollision(Vec3 point);
	/**
	 * Solves the collision between the two bodies.
	 */
	void solveCollision(BodyComponent other);
	/**
	 * Check intersections between the body and a given line.
	 * Returns true if there's a solution such that:
	 * *return_intersection = start + direction * u, with u >= 0.0f
	 */
	bool checkLineIntersection(Vec3 start, Vec3 direction, Vec3 *return_intersection);
};

};

#endif
