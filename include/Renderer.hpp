#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <cstdint>
#include <string>

namespace acc {

class Context;
class Font;
class Color;
class BaseTexture;

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
		virtual void renderRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;
		virtual BaseTexture * loadTexture(Context *context, const std::string& filename, int cell_width, int cell_height) = 0;
		virtual BaseTexture * loadTexture(Context *context, Font *font, const std::string& text, const Color& color, bool anti_aliasing) = 0;
		virtual ~Renderer() = default;
};

};

#endif
