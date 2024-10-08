#ifndef DRAW_RECT_COMPONENT_HPP
#define DRAW_RECT_COMPONENT_HPP

#include "Color.hpp"

namespace acc {

struct DrawRectComponent {
	/**
	 * The position of the rect.
	 */
	float x, y;
	/**
	 * The width and height of the rect.
	 */
	int w, h;
	/**
	 * The layer which the rect will be drawn.
	 * Default: 0.
	 */
	int layer;
	/**
	 * Sets if the camera follows the camera or not.
	 * Default: true.
	 */
	bool follow_camera;
	/**
	 * Sets the color of the rect.
	 * Default: (0x00, 0x00, 0x00, 0x00).
	 */
	Color color;

	DrawRectComponent(void);
	DrawRectComponent(int w, int h, int layer, const Color& color);
};

};

#endif
