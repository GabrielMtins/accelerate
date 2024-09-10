#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "vec2.h"

typedef struct{
	Vec2 position;
	Vec2 scale;
	Vec2 rotation;
	int used;
} TransformComponent;

#endif
