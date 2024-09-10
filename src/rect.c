#include "rect.h"

Rect Rect_Create(double x, double y, double w, double h){
	Rect rect = {
		x, y, w, h
	};

	return rect;
}

Rect Rect_CreateVec2(Vec2 origin, Vec2 size){
	Rect rect = {
		origin.x, origin.y, size.x, size.y
	};

	return rect;
}

int Rect_Intersects(Rect a, Rect b){
	if(a.x > b.x + b.w) return 0;
	if(a.y > b.y + b.h) return 0;
	if(a.x + a.w < b.x) return 0;
	if(a.y + a.h < b.y) return 0;

	return 1;
}
