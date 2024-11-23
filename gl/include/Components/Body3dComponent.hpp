#ifndef BODY_3D_COMPONENT_HPP
#define BODY_3D_COMPONENT_HPP

#include "BrushBuilder.hpp"

namespace acc {

enum HITBOX_TYPE {
	HITBOX_TYPE_NONE = -1,
	HITBOX_TYPE_CYLINDER,
	HITBOX_TYPE_BRUSH,
};

struct Body3dComponent {
	Vec3 position;
	Vec3 velocity;
	Vec3 offset_from_transform;
	Vec3 gravity;

	uint32_t collision_layer;
	uint32_t collision_mask;
	uint32_t collision_trigger;

	struct {
		float height, radius, tolerance;
	} cylinder;

	Body3dComponent(void);
	void setOnCollisionLayer(size_t layer, bool is_on);
	void setOnCollisionMask(size_t layer, bool is_on);
	void setOnCollisionTrigger(size_t layer, bool is_on);

	bool checkCollision(const Body3dComponent& other);
	bool checkCollision(BrushBuilder *brush);
	bool checkCollision(BrushBuilder *brush, const Vec3& position);
	void solveCollision(const Body3dComponent& other);
	bool solveCollision(BrushBuilder *brush);
	bool solveCollision(BrushBuilder *brush, const Vec3& position);
};

};

#endif
