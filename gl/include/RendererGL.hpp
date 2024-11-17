#ifndef RENDERER_GL_HPP
#define RENDERER_GL_HPP

#include "glad/glad.h"
#include "Context.hpp"
#include "Renderer.hpp"

namespace acc {

class Shader;
class Mesh;

class RendererGL : public Renderer {
	public:
		static void init(void);
		RendererGL(Context *context);
		void setWindowSize(int width, int height);
		void setInternalSize(int width, int height);
		void setScaling(RendererScaling scaling);
		void clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void renderPresent(void);
		void renderRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		BaseTexture * loadTexture(Context *context, const std::string& filename, int cell_width, int cell_height);
		BaseTexture * loadTexture(Context *context, Font *font, const std::string& text, const Color& color, bool anti_aliasing);
		void setViewportBestFit(void);
		void setViewportPixelPerfect(void);

		~RendererGL();

		Mesh *default_texture_mesh;
		Shader *default_texture_shader;
	
	private:
		RendererScaling scaling;

		Mesh *default_rectangle_mesh;
		Shader *default_rectangle_shader;

		SDL_GLContext gl_context;
		Context *main_context;

		int internal_width, internal_height;
		int window_width, window_height;
};

};

#endif
