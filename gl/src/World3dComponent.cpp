#include "Components/World3dComponent.hpp"

namespace acc {

World3dComponent::World3dComponent(void){
	collision_layer = 0;
	shader = NULL;
	octree = NULL;
	debug_render_octree = NULL;
}

World3dComponent::World3dComponent(Shader *shader){
	collision_layer = 0;
	this->shader = shader;
	octree = NULL;
	debug_render_octree = NULL;
}

World3dComponent::World3dComponent(Resource *shader){
	collision_layer = 0;
	octree = NULL;

	if(shader == NULL)
		this->shader = NULL;
	else
		this->shader = (Shader *) shader;
	debug_render_octree = NULL;
}

void World3dComponent::setCollisionLayer(int layer){
	collision_layer = collision_layer | (1 << (layer - 1));
}

};
