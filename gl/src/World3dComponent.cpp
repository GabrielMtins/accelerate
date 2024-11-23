#include "Components/World3dComponent.hpp"

namespace acc {

World3dComponent::World3dComponent(void){
	collision_layer = 0;
}

World3dComponent::World3dComponent(Shader *shader){
	collision_layer = 0;
	this->shader = shader;
}

World3dComponent::World3dComponent(Resource *shader){
	collision_layer = 0;

	if(shader == NULL)
		this->shader = NULL;
	else
		this->shader = (Shader *) shader;
}

void World3dComponent::setCollisionLayer(int layer){
	collision_layer = collision_layer | (1 << (layer - 1));
}

};
