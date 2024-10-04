#ifndef DRAW_RECT_COMPONENT_HPP
#define DRAW_RECT_COMPONENT_HPP

#include "Color.hpp"

namespace acc {

struct DrawRectComponent {
	float x, y;
	int w, h;
	int layer;
	bool follow_camera; /* default = true */
	Color color;

	DrawRectComponent(void);
	DrawRectComponent(int w, int h, int layer, Color color);
};

};

#endif
