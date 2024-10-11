#include "Components/BodyComponent.hpp"

#include <cmath>
#include <array>
#include <vector>
#include <algorithm>

namespace acc {

BodyComponent::BodyComponent(void){
	position = Vec3();
	velocity = Vec3();
	offset_from_transform = Vec3();
	size = Vec3();
	gravity = Vec3();

	collision_mask = collision_layer = collision_trigger = 0;
};

void BodyComponent::setOnCollisionLayer(size_t layer, bool is_on){
	if(is_on){
		collision_layer = collision_layer | (1 << (layer - 1));
	}
	else{
		collision_layer = collision_layer & (~(1 << (layer - 1)));
	}
}

void BodyComponent::setOnCollisionMask(size_t layer, bool is_on){
	if(is_on){
		collision_mask = collision_mask | (1 << (layer - 1));
	}
	else{
		collision_mask = collision_mask & (~(1 << (layer - 1)));
	}
}

void BodyComponent::setOnCollisionTrigger(size_t layer, bool is_on){
	if(is_on){
		collision_trigger = collision_trigger | (1 << (layer - 1));
	}
	else{
		collision_trigger = collision_trigger & (~(1 << (layer - 1)));
	}
}

bool BodyComponent::checkCollision(const BodyComponent& other){
	if(!((collision_mask & other.collision_layer) != 0 || (collision_trigger & other.collision_layer) != 0)) return false;

	if(position.x > other.position.x + other.size.x) return false;
	if(position.y > other.position.y + other.size.y) return false;
	if(position.z > other.position.z + other.size.z) return false;

	if(position.x + size.x < other.position.x) return false;
	if(position.y + size.y < other.position.y) return false;
	if(position.z + size.z < other.position.z) return false;

	return true;
}

bool BodyComponent::checkCollision(const Vec3& point){
	if(point.x < position.x) return false;
	if(point.y < position.y) return false;
	if(point.z < position.z) return false;

	if(point.x > position.x + size.x) return false;
	if(point.y > position.y + size.y) return false;
	if(point.z > position.z + size.z) return false;

	return true;
}

void BodyComponent::solveCollision(const BodyComponent& other){
	if(!checkCollision(other))
		return;

	/* if it's just a trigger */
	if((collision_trigger & other.collision_layer) != 0)
		return;

	std::array<Vec3, 4> tmp{position, position + size, other.position, other.position + other.size};
	std::vector<Vec3> min_distance;

	/* just use this to check if the object isn't entirely inside another */
	std::sort(tmp.begin(), tmp.end(), [](Vec3 a, Vec3 b){return a.x < b.x;});
	if(tmp[2].x - tmp[1].x < fmin(size.x, other.size.x)){
		min_distance.push_back((tmp[2] - tmp[1]) * Vec3(1.0f, 0.0f, 0.0f));
	}

	std::sort(tmp.begin(), tmp.end(), [](Vec3 a, Vec3 b){return a.y < b.y;});

	if(tmp[2].y - tmp[1].y < fmin(size.y, other.size.y)){
		min_distance.push_back((tmp[2] - tmp[1]) * Vec3(0.0f, 1.0f, 0.0f));
	}

	std::sort(tmp.begin(), tmp.end(), [](Vec3 a, Vec3 b){return a.z < b.z;});
	if(tmp[2].z - tmp[1].z < fmin(size.z, other.size.z)){
		min_distance.push_back((tmp[2] - tmp[1]) * Vec3(0.0f, 0.0f, 1.0f));
	}

	if(min_distance.size() == 0) return;
	
	std::sort(min_distance.begin(), min_distance.end(),
			[](Vec3 a, Vec3 b){
				if(a.length() == 0.0f) return false;
				return a.lengthSqr() < b.lengthSqr();
			}
			);

	Vec3 direction_hit = other.position - position;

	position += (min_distance[0]) * direction_hit.sign() * (-1.1f);

	/* adjust velocity accordingly */
	Vec3 delta_vel = velocity * velocity.sign() * (min_distance[0] * direction_hit).sign() * (-1.0f);

	/* check if the delta_vel isn't at the same direction as the velocity */
	if(Vec3::dot(delta_vel, velocity) < 0){
		velocity += delta_vel;
	}
}

bool BodyComponent::checkLineIntersection(Vec3 start, Vec3 direction, Vec3 *return_intersection){
	bool found_collision = false;
	std::array<Vec3, 3> normals = {Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1)};

	Vec3 closest_point = start + direction * (1 << 16);

	for(auto& plane_normal : normals){
		float plane_product = Vec3::dot(plane_normal, position);

		if(!Vec3::rectPlaneIntersects(start, direction, plane_normal, plane_product)) continue;

		Vec3 intersection = Vec3::rectPlaneIntersection(
				start,
				direction,
				plane_normal,
				plane_product
				);

		if(checkCollision(intersection)){
			if((intersection - start).lengthSqr() < (closest_point - start).lengthSqr()){
				closest_point = intersection;
			}

			found_collision = true;
		}
	}

	for(auto& plane_normal : normals){
		float plane_product = Vec3::dot(plane_normal, position + size);

		if(!Vec3::rectPlaneIntersects(start, direction, plane_normal, plane_product)) continue;

		Vec3 intersection = Vec3::rectPlaneIntersection(
				start,
				direction,
				plane_normal,
				plane_product
				);

		if(checkCollision(intersection)){
			if((intersection - start).lengthSqr() < (closest_point - start).lengthSqr()){
				closest_point = intersection;
			}

			found_collision = true;
		}
	}

	if(found_collision && return_intersection != NULL)
		*return_intersection = closest_point;

	return found_collision;
}

};
