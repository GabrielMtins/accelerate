#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <cstdint>

namespace acc {

enum RendererScaling {
	RENDERER_SCALING_BESTFIT = 0,
	RENDERER_SCALING_PIXELPERFECT,
	RENDERER_SCALING_EXPAND,
};

class Renderer {
	public:
		virtual void setWindowSize(int width, int height) = 0;
		virtual void setInternalSize(int width, int height) = 0;
		virtual void setScaling(RendererScaling scaling) = 0;
		virtual void clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;
		virtual void renderPresent(void) = 0;
		virtual ~Renderer() = 0;
};

};

#endif
