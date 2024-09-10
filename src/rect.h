#ifndef RECT_H
#define RECT_H

#include "vec2.h"

typedef struct{
	double x, y, w, h;
} Rect;

Rect Rect_Create(double x, double y, double w, double h);

Rect Rect_CreateVec2(Vec2 origin, Vec2 size);

int Rect_Intersects(Rect a, Rect b);

#endif
