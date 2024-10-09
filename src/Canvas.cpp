#include "Canvas.hpp"

#include <math.h>

namespace acc {

Canvas::Canvas(const std::string& resource_name){
	setName(resource_name);
	surface = IMG_Load(resource_name.c_str());

	if(surface == NULL){
		fprintf(stderr, "Failed to load texture: %s\n", resource_name.c_str());
	}

	pixels = (uint32_t *) surface->pixels;
}

Canvas::Canvas(const std::string& resource_name, int width, int height){
	setName(resource_name);

	surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	clear(Color(0x00, 0x00, 0x00));
	pixels = (uint32_t *) surface->pixels;
}

int Canvas::getWidth(void){
	return surface->w;
}

int Canvas::getHeight(void){
	return surface->h;
}

SDL_Surface * Canvas::getSurface(void){
	return surface;
}

Color Canvas::getColorNormal(float x, float y){
	x = fmodf(x, 1.0f);
	y = fmodf(y, 1.0f);

	if(x < 0) x += 1.0f;
	if(y < 0) y += 1.0f;

	return getColor((int) (x * surface->w), (int) (y * surface->h));
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
	if(x < 0 || y < 0 || x >= surface->w || y >= surface->h)
		return;

	pixels[getPixelId(x, y)] = SDL_MapRGBA(
			surface->format,
			c.r,
			c.g,
			c.b,
			c.a
			);
}

void Canvas::drawLine(const Color& c, int x1, int y1, int x2, int y2){
	int dx = (x2 - x1);
	int dy = (y2 - y1);

	int steps = std::max(abs(dx), abs(dy));

	float step_x = (float) dx / steps;
	float step_y = (float) dy / steps;

	float start_x = x1;
	float start_y = y1;

	for(int i = 0; i < steps; i++){
		setColor(c, start_x, start_y);
		start_x += step_x;
		start_y += step_y;
	}
}

void Canvas::drawCircle(const Color& c, int x, int y, int radius){
	for(int i = -radius + 1; i < radius; i++){
		for(int j = -radius + 1; j < radius; j++){
			if(i * i + j * j > radius * radius) continue;

			setColor(c, x + i, y + j);
		}
	}
}

void Canvas::clear(const Color& c){
	SDL_FillRect(
			surface,
			NULL,
			SDL_MapRGBA(
					surface->format,
					c.r,
					c.g,
					c.b,
					c.a
					)
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
