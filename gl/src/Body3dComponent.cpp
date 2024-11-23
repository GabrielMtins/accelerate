#include "Components/Body3dComponent.hpp"

namespace acc {

Body3dComponent::Body3dComponent(void){
	position = Vec3();
	velocity = Vec3();
	offset_from_transform = Vec3();
	gravity = Vec3();

	collision_layer = collision_mask = collision_trigger = 0;
	
	cylinder.height = cylinder.radius = cylinder.tolerance = 0.0f;
}

void Body3dComponent::setOnCollisionLayer(size_t layer, bool is_on){
	if(is_on){
		collision_layer = collision_layer | (1 << (layer - 1));
	}
	else{
		collision_layer = collision_layer & (~(1 << (layer - 1)));
	}
}

void Body3dComponent::setOnCollisionMask(size_t layer, bool is_on){
	if(is_on){
		collision_mask = collision_mask | (1 << (layer - 1));
	}
	else{
		collision_mask = collision_mask & (~(1 << (layer - 1)));
	}
}

void Body3dComponent::setOnCollisionTrigger(size_t layer, bool is_on){
	if(is_on){
		collision_trigger = collision_trigger | (1 << (layer - 1));
	}
	else{
		collision_trigger = collision_trigger & (~(1 << (layer - 1)));
	}
}

bool Body3dComponent::checkCollision(const Body3dComponent& other){
	if(!((collision_mask & other.collision_layer) != 0 || (collision_trigger & other.collision_layer) != 0))
		return false;

	if(position.y > other.position.y + other.cylinder.height)
		return false;

	if(position.y + cylinder.height < other.position.y)
		return false;

	float distance_sqr = ((position - other.position) * Vec3(1.0f, 0.0f, 1.0f)).lengthSqr();
	float max_distance = cylinder.radius + other.cylinder.radius;

	if(distance_sqr >= max_distance * max_distance)
		return false;

	return true;
}

bool Body3dComponent::checkCollision(BrushBuilder *brush){
	return brush->checkCollisionCylinder(
			position,
			cylinder.radius,
			cylinder.height,
			cylinder.tolerance,
			NULL
			);
}

bool Body3dComponent::checkCollision(BrushBuilder *brush, const Vec3& position){
	bool found_collision;

	this->position -= position;
	found_collision =  brush->checkCollisionCylinder(
			this->position - position,
			cylinder.radius,
			cylinder.height,
			cylinder.tolerance,
			NULL
			);
	this->position += position;

	return found_collision;
}

void Body3dComponent::solveCollision(const Body3dComponent& other){
	if(!checkCollision(other))
		return;

	if((collision_trigger & other.collision_layer) != 0)
		return;

	Vec3 direction = ((position - other.position) * Vec3(1.0f, 0.0f, 1.0f)).normalize();
	Vec3 delta = direction * (other.cylinder.radius + cylinder.radius) * 1.01f;

	float old_y = position.y;
	position = other.position + delta;
	position.y = old_y;
}

bool Body3dComponent::solveCollision(BrushBuilder *brush){
	Vec3 delta;
	bool found_collision = false;
	
	found_collision = brush->checkCollisionCylinder(
			position,
			cylinder.radius,
			cylinder.height,
			cylinder.tolerance,
			&delta
			);

	if(found_collision){
		position += delta * 1.01f;

		if(delta.lengthSqr() > 0.00f && Vec3::dot(delta, velocity) < 0.0f){
			velocity -= delta * (Vec3::dot(delta, velocity) / delta.lengthSqr()) * 1.00f;
		}
	}

	return found_collision;
}

bool Body3dComponent::solveCollision(BrushBuilder *brush, const Vec3& position){
	bool found_collision;

	this->position -= position;
	found_collision = solveCollision(brush);
	this->position += position;

	return found_collision;
}

};
