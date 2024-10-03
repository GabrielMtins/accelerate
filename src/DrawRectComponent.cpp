#include "Components/DrawRectComponent.hpp"

namespace acc {

DrawRectComponent::DrawRectComponent(void){
	x = y = w = h = 0;
	follow_camera = false;
	layer = 0;
	color = Color(0x00, 0x00, 0x00, 0x00);
}

DrawRectComponent::DrawRectComponent(int x, int y, int w, int h, int layer, Color color){
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->layer = layer;
	this->color = color;
	follow_camera = false;
}

};
