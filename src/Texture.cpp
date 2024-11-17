#include "Texture.hpp"
#include "RendererSDL.hpp"

#include <algorithm>

namespace acc {

static SDL_Vertex Vertex_ConvertToSDL(Vertex vertex);

Texture::Texture(Context *context, const std::string& filename, Canvas* canvas){
	setName(filename);

	renderer = ((RendererSDL *) context->getRenderer())->getSDLRenderer();

	texture = SDL_CreateTextureFromSurface(
			renderer,
			canvas->getSurface()
			);

	texture_width = cell_width = canvas->getWidth();
	texture_height = cell_height = canvas->getHeight();
}

Texture::Texture(Context *context, const std::string& filename, int cell_width, int cell_height){
	setName(filename);

	SDL_Surface *surface = IMG_Load(filename.c_str());
	renderer = ((RendererSDL *) context->getRenderer())->getSDLRenderer();

	if(surface == NULL){
		fprintf(stderr, "Failed to load texture: %s\n", filename.c_str());
		texture = NULL;
	}
	else{
		texture = SDL_CreateTextureFromSurface(renderer, surface);

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

Texture::Texture(Context *context, Font *font, const std::string& text, const Color& color, bool anti_aliasing){
	if(text == ""){
		texture = NULL;
		return;
	}

	SDL_Color fg = {color.r, color.g, color.b, color.a};
	SDL_Surface *surface = NULL;

	renderer = ((RendererSDL *) context->getRenderer())->getSDLRenderer();

	if(anti_aliasing)
		surface = TTF_RenderUTF8_Blended_Wrapped(font->getFont(), text.c_str(), fg, 0);
	else
		surface = TTF_RenderUTF8_Solid_Wrapped(font->getFont(), text.c_str(), fg, 0);


	if(surface == NULL){
		texture = NULL;
		return;
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	
	texture_width = surface->w;
	texture_height = surface->h;

	cell_width = surface->w;
	cell_height = surface->h;

	SDL_FreeSurface(surface);
}

void Texture::updateCanvas(Context *context, Canvas *canvas){
	if(texture != NULL)
		SDL_DestroyTexture(texture);

	texture = SDL_CreateTextureFromSurface(
			renderer,
			canvas->getSurface()
			);
}

void Texture::renderCell(Context *context, int x, int y, int id){
	(void) context;

	SDL_Rect src_rect = getIdRect(id);
	SDL_Rect dst_rect = getDstRect(x, y, id);

	SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);
}

void Texture::renderCellEx(Context *context, int x, int y, int id, float scale_x, float scale_y, int center_x, int center_y, float angle, bool flip_x, bool flip_y){
	(void) context;

	if(context == NULL || texture == NULL) return;

	SDL_Rect src_rect = getIdRect(id);
	SDL_Rect dst_rect = getDstRect(x, y, id);

	dst_rect.w *= scale_x;
	dst_rect.h *= scale_y;

	center_x *= scale_x;
	center_y *= scale_y;

	SDL_Point center_sdl = {center_x, center_y};
	int flip = SDL_FLIP_NONE;

	if(flip_x)
		flip = flip | SDL_FLIP_HORIZONTAL;

	if(flip_y)
		flip = flip | SDL_FLIP_VERTICAL;

	SDL_RenderCopyEx(
			renderer,
			texture,
			&src_rect,
			&dst_rect,
			angle,
			&center_sdl,
			(SDL_RendererFlip) flip
			);
}

void Texture::renderRect(Context *context, int src[], int dst[]){
	(void) context;

	if(context == NULL || texture == NULL) return;

	SDL_Rect src_rect, dst_rect;

	if(src == NULL){
		src_rect.x = src_rect.y = 0;
		src_rect.w = texture_width;
		src_rect.h = texture_height;
	}
	else{
		src_rect.x = src[0];
		src_rect.y = src[1];
		src_rect.w = src[2];
		src_rect.h = src[3];
	}

	if(dst == NULL){
		SDL_RenderCopy(renderer, texture, &src_rect, NULL);
	}
	else{
		dst_rect.x = dst[0];
		dst_rect.y = dst[1];
		dst_rect.w = dst[2];
		dst_rect.h = dst[3];
		SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);
	}

}

int Texture::getTextureWidth(void){
	return texture_width;
}

int Texture::getTextureHeight(void){
	return texture_height;
}

int Texture::getCellWidth(void){
	return cell_width;
}

int Texture::getCellHeight(void){
	return cell_height;
}

Texture::~Texture(void){
	if(texture != NULL)
		SDL_DestroyTexture(texture);
}

SDL_Rect Texture::getIdRect(int id){
	if(id == -1){
		SDL_Rect src_rect = {0, 0, texture_width, texture_height};

		return src_rect;
	}

	int num_tex_x = texture_width / cell_width;

	int id_x = (id % num_tex_x);
	int id_y = (id / num_tex_x);

	SDL_Rect src_rect = {
		id_x * cell_width,
		id_y * cell_height,
		cell_width,
		cell_height
	};

	return src_rect;
}

SDL_Rect Texture::getDstRect(int x, int y, int id){
	SDL_Rect dst_rect;

	dst_rect.x = x;
	dst_rect.y = y;

	if(id == -1){
		dst_rect.w = texture_width;
		dst_rect.h = texture_height;
	}
	else{
		dst_rect.w = cell_width;
		dst_rect.h = cell_height;
	}

	return dst_rect;
}

static SDL_Vertex Vertex_ConvertToSDL(Vertex vertex){
	uint8_t alpha = (uint8_t) (vertex.alpha * 255.0f);
	vertex.color *= 255.0f;

	SDL_Vertex transformed = {
		{vertex.position.x, vertex.position.y},
		{(uint8_t) vertex.color.x, (uint8_t) vertex.color.y, (uint8_t) vertex.color.z, alpha},
		{vertex.uv.x, vertex.uv.y}
	};

	return transformed;
}

};
