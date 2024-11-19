#include "Components/Sprite3d.hpp"

namespace acc {

Sprite3dComponent::Sprite3dComponent(Shader *shader, TextureGL *texture){
	id = 0;
	flip_horizontally = true;
	flip_vertically = true;

	scale = Mat4::Identity();
	rotation = Mat4::Identity();

	this->shader = shader;
	this->texture = texture;
}

Sprite3dComponent::Sprite3dComponent(Resource *shader, Resource *texture){
	id = 0;
	flip_horizontally = true;
	flip_vertically = true;

	scale = Mat4::Identity();
	rotation = Mat4::Identity();

	if(shader == NULL)
		this->shader = NULL;
	else
		this->shader = (Shader *) shader;

	if(texture == NULL)
		this->texture = NULL;
	else
		this->texture = (TextureGL *) texture;
}

};
