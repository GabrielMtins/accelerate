#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "Resource.hpp"
#include "Context.hpp"
#include "Mesh.hpp"
#include "Color.hpp"

namespace acc {

class Canvas : public Resource {
	public:
		Canvas(std::string resource_name);
		Canvas(std::string resource_name, int width, int height);

		SDL_Surface * getSurface(void);
		Color getColorNormal(float x, float y);
		Color getColor(int x, int y);
		void setColor(const Color& c, int x, int y);
		void clear(const Color& c);

		~Canvas(void);

	private:
		uint32_t getPixelId(int x, int y);

		SDL_Surface *surface;
		uint32_t *pixels;
};

};

#endif
