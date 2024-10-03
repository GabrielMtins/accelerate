#ifndef DRAW_RECT_COMPONENT_HPP
#define DRAW_RECT_COMPONENT_HPP

#include "Color.hpp"

namespace acc {

struct DrawRectComponent {
	int x, y, w, h;
	int layer;
	bool follow_camera;
	Color color;

	DrawRectComponent(void);
	DrawRectComponent(int x, int y, int w, int h, int layer, Color color);
};

};

#endif
