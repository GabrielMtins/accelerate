#include "Components/Sprite.hpp"

namespace acc {

SpriteComponent::SpriteComponent(void){
	texture = NULL;
	id = -1;
	layer = 0;
}

SpriteComponent::SpriteComponent(Texture *texture){
	this->texture = texture;
	id = -1;
	layer = 0;
}

};
