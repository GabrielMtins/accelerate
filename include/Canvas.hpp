#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "Resource.hpp"
#include "Context.hpp"
#include "Mesh.hpp"
#include "Color.hpp"

namespace acc {

class Canvas : public Resource {
	public:
		/**
		 * Loads a Canvas from the file named "resource_name".
		 */
		Canvas(const std::string& resource_name);
		/**
		 * Creates a canvas with a custom name and custom width and height.
		 */
		Canvas(const std::string& resource_name, int width, int height);
		int getWidth(void);
		int getHeight(void);
		SDL_Surface * getSurface(void);
		/**
		 * On this case, the coordinates gets normalized between 0.0f and 1.0f.
		 * If the x or y aren't between 0.0f and 1.0f, the coordinates will be
		 * be divided for the remainder.
		 */
		Color getColorNormal(float x, float y);
		/**
		 * Returns the color at pixel (x, y).
		 */
		Color getColor(int x, int y);
		/**
		 * Sets the Color c for the pixel (x, y).
		 */
		void setColor(const Color& c, int x, int y);
		/**
		 * Draws a line with Color c between the points (x1, y1) and (x2, y2).
		 */
		void drawLine(const Color& c, int x1, int y1, int x2, int y2);
		/**
		 * Draws a circle with center at (x, y) and radius given.
		 */
		void drawCircle(const Color& c, int x, int y, int radius);
		/**
		 * Clears the canvas with a given color.
		 */
		void clear(const Color& c);

		~Canvas(void);

	private:
		uint32_t getPixelId(int x, int y);

		SDL_Surface *surface;
		uint32_t *pixels;
};

};

#endif
