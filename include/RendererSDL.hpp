#ifndef RENDERER_SDL_HPP
#define RENDERER_SDL_HPP

#include "Context.hpp"
#include "Renderer.hpp"

namespace acc {

class RendererSDL : public Renderer {
	public:
		RendererSDL(Context *context);
		void setWindowSize(int width, int height);
		void setInternalSize(int width, int height);
		void setScaling(RendererScaling scaling);
		void clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void renderPresent(void);
		~RendererSDL();

	private:
		void renderPixelPerfect(void);
		void renderBestFit(void);

		SDL_Renderer *renderer;
		SDL_Texture *framebuffer;

		int internal_width, internal_height;
		int window_width, window_height;
		
		RendererScaling scaling;
};

};

#endif
