#include "Components/Object3dComponent.hpp"

namespace acc {

Object3dComponent::Object3dComponent(Model3d *model3d, Shader *shader){
	model = Mat4::Identity();

	this->model3d = model3d;
	this->shader = shader;
}

Object3dComponent::Object3dComponent(Model3d *model3d, Resource *shader){
	model = Mat4::Identity();

	this->model3d = model3d;

	if(shader == NULL)
		this->shader = NULL;
	else
		this->shader = (Shader *) shader;
}

};
