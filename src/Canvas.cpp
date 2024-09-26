#include "Canvas.hpp"

namespace acc {

Canvas::Canvas(std::string resource_name, int width, int height){
	setName(resource_name);

	surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	pixels = (uint32_t *) surface->pixels;
}

SDL_Surface * Canvas::getSurface(void){
	return surface;
}

Color Canvas::getColor(int x, int y){
	Color return_color = Color();

	if(x < 0 || y < 0 || x >= surface->w || y >= surface->h)
		return return_color;

	SDL_GetRGBA(
			pixels[getPixelId(x, y)],
			surface->format,
			&return_color.r,
			&return_color.g,
			&return_color.b,
			&return_color.a
			);

	return return_color;
}

void Canvas::setColor(const Color& c, int x, int y){
	pixels[getPixelId(x, y)] = SDL_MapRGBA(
			surface->format,
			c.r,
			c.g,
			c.b,
			c.a
			);
}

Canvas::~Canvas(void){
	pixels = NULL;
	SDL_FreeSurface(surface);
}

uint32_t Canvas::getPixelId(int x, int y){
	return x + y * surface->w;
}

};