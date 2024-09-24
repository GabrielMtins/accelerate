#include "Components/Sprite.hpp"

namespace acc {

SpriteComponent::SpriteComponent(void){
	texture = NULL;
	id = -1;
	layer = 0;
	follow_camera = true;

	scale = Vec3(1.0f, 1.0f, 1.0f);
	center = Vec3(0.0f, 0.0f, 0.0f);
	angle = 0.0f;
}

SpriteComponent::SpriteComponent(Resource *texture){
	if(texture == NULL)
		this->texture = NULL;
	else
		this->texture = (Texture *) texture;

	id = -1;
	layer = 0;
	follow_camera = true;

	scale = Vec3(1.0f, 1.0f, 1.0f);
	center = Vec3(0.0f, 0.0f, 0.0f);
	angle = 0.0f;
}

SpriteComponent::SpriteComponent(Texture *texture){
	if(texture == NULL)
		this->texture = NULL;
	else
		this->texture = (Texture *) texture;

	id = -1;
	layer = 0;
	follow_camera = true;

	scale = Vec3(1.0f, 1.0f, 1.0f);
	center = Vec3(0.0f, 0.0f, 0.0f);
	angle = 0.0f;
}

};
