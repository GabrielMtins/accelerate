#include "RendererGL.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "TextureGL.hpp"

namespace acc {

static const char *default_rectangle_vertex_src =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 tex_coords;\n"
"uniform vec4 offset;\n"
"void main(){\n"
"	gl_Position = vec4(\n"
"			aPos.x * offset.z + offset.x,\n"
"			-(aPos.y * offset.w + offset.y),\n"
"			0.0f,\n"
"			1.0f\n"
"			);\n"
"}\n";

static const char *default_rectangle_fragment_src = 
"#version 330 core\n"
"out vec4 FragColor;\n"

"uniform vec4 color;\n"

"void main(){\n"
"	FragColor = color;\n"
"}\n";

static const char *default_texture_vertex_src =
"#version 330 core\n"
"layout (location = 0) in vec3 in_pos;\n"
"layout (location = 1) in vec2 in_tex_coords;\n"
"uniform vec4 offset_tex_coords;\n"
"uniform mat4 model;\n"
"out vec2 out_tex_coords;\n"
"void main(){\n"
	"gl_Position = model * vec4(in_pos, 1.0f);\n"
	"gl_Position.y *= -1.0f;\n"
	"gl_Position.z = 0.0f;\n"
	"out_tex_coords = vec2(\n"
			"in_tex_coords.x * offset_tex_coords.z + offset_tex_coords.x,\n"
			"in_tex_coords.y * offset_tex_coords.w + offset_tex_coords.y\n"
	");\n"
"}\n";

static const char *default_texture_fragment_src =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 out_tex_coords;\n"
"uniform sampler2D frag_texture;\n"
"void main(){\n"
	"FragColor = texture(frag_texture, out_tex_coords);\n"
"}\n";

void RendererGL::init(void){
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
}

RendererGL::RendererGL(Context *context){
	main_context = context;
	gl_context = SDL_GL_CreateContext(context->getWindow());

	if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
        fprintf(stderr, "Failed to initialize GLAD\n");
	}

	default_rectangle_mesh = new Mesh();
	default_rectangle_shader = new Shader();

	default_texture_mesh = new Mesh();
	default_texture_shader = new Shader();

	default_rectangle_mesh->load(
			{
				{0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
				{1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
				{0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
				{1.0f, 1.0f, 0.0f, 1.0f, 1.0f}
			},
			{0, 1, 2, 2, 3, 1}
			);

	default_rectangle_shader->loadShader(
			default_rectangle_vertex_src,
			default_rectangle_fragment_src
			);

	default_texture_mesh->load(
			{
				{0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
				{1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
				{0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
				{1.0f, 1.0f, 0.0f, 1.0f, 1.0f}
			},
			{0, 1, 2, 2, 3, 1}
			);

	default_texture_shader->loadShader(
			default_texture_vertex_src,
			default_texture_fragment_src
			);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	scaling = RENDERER_SCALING_BESTFIT;
}

void RendererGL::setWindowSize(int width, int height){
	window_width = width;
	window_height = height;


	switch(scaling){
		case RENDERER_SCALING_EXPAND:
			glViewport(0, 0, width, height);
			break;

		case RENDERER_SCALING_PIXELPERFECT:
			setViewportPixelPerfect();
			break;

		case RENDERER_SCALING_BESTFIT:
			setViewportBestFit();
			break;
	}
}

void RendererGL::setInternalSize(int width, int height){
	glViewport(0, 0, width, height);

	internal_width = width;
	internal_height = height;
}

void RendererGL::setScaling(RendererScaling scaling){
	this->scaling = scaling;
}

void RendererGL::clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	glClearColor((float) r / 255.0f, (float) g / 255.0f, (float) b / 255.0f, (float) a / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererGL::renderPresent(void){
	SDL_GL_SwapWindow(main_context->getWindow());
}

void RendererGL::renderRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	default_rectangle_shader->setUniform(
			"offset",
			((float) x) / (internal_width / 2) - 1.0f,
			((float) y) / (internal_height / 2) - 1.0f,
			((float) w) / (internal_width / 2),
			((float) h) / (internal_height / 2)
			);

	default_rectangle_shader->setUniform(
			"color",
			((float) r) / 255.0f,
			((float) g) / 255.0f,
			((float) b) / 255.0f,
			((float) a) / 255.0f
			);

	default_rectangle_mesh->render(default_rectangle_shader);
}

BaseTexture * RendererGL::loadTexture(Context *context, const std::string& filename, int cell_width, int cell_height){
	return new TextureGL(context, filename, cell_width, cell_height);
}

BaseTexture * RendererGL::loadTexture(Context *context, Font *font, const std::string& text, const Color& color, bool anti_aliasing){
	return new TextureGL(context, font, text, color, anti_aliasing);
}

void RendererGL::setViewportBestFit(void){
	int defacto_width, defacto_height;

	if(window_width * internal_height < window_height * internal_width){
		defacto_width = window_width;
		defacto_height = internal_height * window_width / internal_width;
	}
	else{
		defacto_width = internal_width * window_height / internal_height;
		defacto_height = window_height;
	}

	glViewport(
			(window_width - defacto_width) / 2,
			(window_height - defacto_height) / 2,
			defacto_width,
			defacto_height
		);
}

void RendererGL::setViewportPixelPerfect(void){
	int scale_factor = std::min(window_width / internal_width, window_height / internal_height);

	int defacto_width = scale_factor * internal_width;
	int defacto_height = scale_factor * internal_height;

	glViewport(
			(window_width - defacto_width) / 2,
			(window_height - defacto_height) / 2,
			defacto_width,
			defacto_height
		);
}

RendererGL::~RendererGL(){
	delete default_rectangle_shader;
	delete default_rectangle_mesh;

	delete default_texture_shader;
	delete default_texture_mesh;

	SDL_GL_DeleteContext(gl_context);
}

};

