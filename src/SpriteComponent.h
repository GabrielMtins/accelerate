#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "texture.h"

typedef struct{
	Vec2 position;
	Texture *texture;
	int id;
	int layer;
	int used;
} SpriteComponent;

#endif
