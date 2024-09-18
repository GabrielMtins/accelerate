#include "Components/BodyComponent.hpp"

#include <array>
#include <algorithm>

namespace acc {

BodyComponent::BodyComponent(void){
	position = Vec3();
	velocity = Vec3();
	offset_from_transform = Vec3();
	size = Vec3();
	gravity = Vec3();

	is_trigger = false;

	collision_mask = collision_layer = 0;
};

void BodyComponent::setOnCollisionLayer(size_t layer, bool is_on){
	if(is_on){
		collision_layer = collision_layer | (1 << layer);
	}
	else{
		collision_layer = collision_layer & (~(1 << layer));
	}
}

void BodyComponent::setOnCollisionMask(size_t layer, bool is_on){
	if(is_on){
		collision_mask = collision_mask | (1 << layer);
	}
	else{
		collision_mask = collision_mask & (~(1 << layer));
	}
}

bool BodyComponent::checkCollision(BodyComponent other){
	if((collision_mask & other.collision_layer) == 0) return false;

	if(position.x > other.position.x + other.size.x) return false;
	if(position.y > other.position.y + other.size.y) return false;
	if(position.z > other.position.z + other.size.z) return false;

	if(position.x + size.x < other.position.x) return false;
	if(position.y + size.y < other.position.y) return false;
	if(position.z + size.z < other.position.z) return false;

	return true;
}

void BodyComponent::solveCollision(BodyComponent other){
	if(!checkCollision(other))
		return;

	if(is_trigger)
		return;
	
	std::array<Vec3, 4> tmp{position, position + size, other.position, other.position + other.size};
	std::array<Vec3, 3> min_distance;

	std::sort(tmp.begin(), tmp.end(), [](Vec3 a, Vec3 b){return a.x < b.x;});
	min_distance[0] = tmp[2] - tmp[1];
	min_distance[0].y = min_distance[0].z = 0;

	std::sort(tmp.begin(), tmp.end(), [](Vec3 a, Vec3 b){return a.y < b.y;});
	min_distance[1] = tmp[2] - tmp[1];
	min_distance[1].x = min_distance[1].z = 0;

	std::sort(tmp.begin(), tmp.end(), [](Vec3 a, Vec3 b){return a.z < b.z;});
	min_distance[2] = tmp[2] - tmp[1];
	min_distance[2].x = min_distance[2].y = 0;

	std::sort(min_distance.begin(), min_distance.end(),
			[](Vec3 a, Vec3 b){
				if(a.length() == 0.0f) return false;
				return a.lengthSqr() < b.lengthSqr();
			}
			);

	Vec3 direction_hit = other.position - position;

	position -= (min_distance[0]) * direction_hit.sign() * (1.1f);

	velocity *= (Vec3(1.0f, 1.0f, 1.0f) - (min_distance[0] * direction_hit).sign()) * 1.0f;

	/*
	printf("\n");
	min_distance[0].print();
	min_distance[1].print();
	*/
}

};
