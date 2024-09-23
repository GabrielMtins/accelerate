#include "Components/Sprite.hpp"

namespace acc {

SpriteComponent::SpriteComponent(void){
	texture = NULL;
	id = -1;
	layer = 0;
	follow_camera = true;
}

SpriteComponent::SpriteComponent(Resource *texture){
	this->texture = (Texture *) texture;
	id = -1;
	layer = 0;
	follow_camera = true;
}

SpriteComponent::SpriteComponent(Texture *texture){
	this->texture = texture;
	id = -1;
	layer = 0;
	follow_camera = true;
}

};
