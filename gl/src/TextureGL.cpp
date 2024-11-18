#include "TextureGL.hpp"
#include "Context.hpp"
#include "RendererGL.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"

#include "Mat4.hpp"

namespace acc {

TextureGL::TextureGL(Context *context, const std::string& filename, int cell_width, int cell_height){
	(void) context;

	setName(filename);
	loaded = true;

	SDL_Surface *surface = IMG_Load(filename.c_str());

	if(surface == NULL){
		fprintf(stderr, "Failed to load texture: %s\n", filename.c_str());
		loaded = false;
	}
	else{
		SDL_Surface *default_format = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
		SDL_FreeSurface(surface);
		surface = default_format;

		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
				surface->w,
				surface->h,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				surface->pixels
				);

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		texture_width = surface->w;
		texture_height = surface->h;

		this->cell_width = cell_width;
		this->cell_height = cell_height;

		if(this->cell_width < 0)
			this->cell_width = surface->w;

		if(this->cell_height < 0)
			this->cell_height = surface->h;

		SDL_FreeSurface(surface);
	}

}

TextureGL::TextureGL(Context *context, Font *font, const std::string& text, const Color& color, bool anti_aliasing){
	(void) context;

	loaded = true;

	if(text == ""){
		loaded = false;
		return;
	}

	SDL_Color fg = {color.r, color.g, color.b, color.a};
	SDL_Surface *surface = NULL;

	if(anti_aliasing)
		surface = TTF_RenderUTF8_Blended_Wrapped(font->getFont(), text.c_str(), fg, 0);
	else
		surface = TTF_RenderUTF8_Solid_Wrapped(font->getFont(), text.c_str(), fg, 0);

	if(surface == NULL){
		loaded = false;
		return;
	}

	SDL_Surface *default_format = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
	SDL_FreeSurface(surface);
	surface = default_format;

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			surface->w,
			surface->h,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			surface->pixels
			);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	texture_width = surface->w;
	texture_height = surface->h;

	this->cell_width = cell_width;
	this->cell_height = cell_height;

	if(this->cell_width < 0)
		this->cell_width = surface->w;

	if(this->cell_height < 0)
		this->cell_height = surface->h;

	SDL_FreeSurface(surface);

}

void TextureGL::renderCell(Context *context, int x, int y, int id){
	renderCellEx(
			context,
			x,
			y,
			id,
			1.0f,
			1.0f,
			0.5f * cell_width,
			0.5f * cell_height,
			0.0f,
			false,
			false
			);
}

void TextureGL::renderCellEx(Context *context, int x, int y, int id, float scale_x, float scale_y, int center_x, int center_y, float angle, bool flip_x, bool flip_y){
	if(!loaded)
		return;

	angle = angle / 180 * 3.141592;

	RendererGL *renderer = (RendererGL *) context->getRenderer();

	Mat4 model = Mat4::Identity();

	/*
	float center_fx = 2.0f * center_x / context->getWidth();
	float center_fy = 2.0f * center_y / context->getHeight();
	float aspect_ratio = context->getWidth() / context->getHeight();

	float ix, iy, iw, ih;

	if(id == -1)
		model = Mat4::Scale(2.0f * texture_width / context->getWidth(), 2.0f * texture_height / context->getHeight(), 0.0f) * model;
	else
		model = Mat4::Scale(2.0f * cell_width / context->getWidth(), 2.0f * cell_height / context->getHeight(), 0.0f) * model;

	model = Mat4::Transform(-center_fx, -center_fy, 0.0f) * model;
	model = Mat4::RotateZ(angle) * model;
	model = Mat4::Transform(center_fx, center_fy, 0.0f) * model;
	model = Mat4::Scale(scale_x, scale_y, 0.0f) * model;
	model = Mat4::Transform(2.0f * x / context->getWidth() - 1.0f, 2.0f * y / context->getHeight() - 1.0f, 0.0f) * model;
	*/

	float center_fx = center_x;
	float center_fy = center_y;

	float ix, iy, iw, ih;

	if(id == -1)
		model = Mat4::Scale(texture_width, texture_height, 0.0f) * model;
	else
		model = Mat4::Scale(cell_width, cell_height, 0.0f) * model;

	model = Mat4::Transform(-center_fx, -center_fy, 0.0f) * model;
	model = Mat4::RotateZ(angle) * model;
	model = Mat4::Transform(center_fx, center_fy, 0.0f) * model;
	model = Mat4::Scale(scale_x, scale_y, 0.0f) * model;
	model = Mat4::Transform(x, y, 0.0f) * model;
	model = Mat4::Scale(2.0f / context->getWidth(), 2.0f / context->getHeight(), 1.0f) * model;
	model = Mat4::Transform(-1.0f, -1.0f, 0.0f) * model;

	renderer->default_texture_shader->setUniform("model", model);
	
	if(id == -1){
		ix = iy = 0.0f;
		iw = 1.0f;
		ih = 1.0f;

		if(flip_x){
			ix = 1.0f;
			iw = -1.0f;
		}

		if(flip_y){
			iy = 1.0f;
			ih = -1.0f;
		}
	}
	else{
		iw = (float) cell_width / texture_width;
		ih = (float) cell_height / texture_height;

		int num_tex_x = texture_width / cell_width;

		int id_x = (id % num_tex_x);
		int id_y = (id / num_tex_x);

		ix = (float) id_x * cell_width / texture_width;
		iy = (float) id_y * cell_height / texture_height;

		if(flip_x){
			ix += (float) cell_width / texture_width;
			iw *= -1.0f;
		}

		if(flip_y){
			iy += (float) cell_height / texture_height;
			ih *= -1.0f;
		}
	}

	renderer->default_texture_shader->setUniform("offset_tex_coords", ix, iy, iw, ih);

	glBindTexture(GL_TEXTURE_2D, texture_id);
	renderer->default_texture_mesh->render(renderer->default_texture_shader);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureGL::renderRect(Context *context, int src[], int dst[]){
}

int TextureGL::getTextureWidth(void){
	return texture_width;
}

int TextureGL::getTextureHeight(void){
	return texture_height;
}

int TextureGL::getCellWidth(void){
	return cell_width;
}

int TextureGL::getCellHeight(void){
	return cell_height;
}

TextureGL::~TextureGL(void){
	if(loaded){
		glDeleteTextures(1, &texture_id);
	}
}

};
