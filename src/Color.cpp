#include "Color.hpp"

namespace acc {

Color::Color(void){
	r = g = b = a = 0x00;
}

Color::Color(uint8_t r, uint8_t g, uint8_t b){
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 0xff;
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

};
