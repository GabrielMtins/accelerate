#include "Components/DrawRectComponent.hpp"

namespace acc {

DrawRectComponent::DrawRectComponent(void){
	x = y = w = h = 0;
	follow_camera = true;
	layer = 0;
	color = Color(0x00, 0x00, 0x00, 0x00);
}

DrawRectComponent::DrawRectComponent(int w, int h, int layer, const Color& color){
	x = 0;
	y = 0;
	this->w = w;
	this->h = h;
	this->layer = layer;
	this->color = color;
	follow_camera = true;
}

};
