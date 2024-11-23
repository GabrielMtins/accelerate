#include "Components/MovBrush3dComponent.hpp"

namespace acc {

MovBrush3dComponent::MovBrush3dComponent(BrushBuilder *brush, Shader *shader){
	this->brush = brush;
	this->shader = shader;
	collision_layer = 0;
}

MovBrush3dComponent::MovBrush3dComponent(BrushBuilder *brush, Resource *shader){
	this->brush = brush;

	if(shader == NULL)
		this->shader = NULL;
	else
		this->shader = (Shader *) shader;

	collision_layer = 0;
}

void MovBrush3dComponent::setCollisionLayer(int layer){
	collision_layer = collision_layer | (1 << (layer - 1));
}

};
