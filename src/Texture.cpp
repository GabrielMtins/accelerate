#include "Texture.hpp"

#include <algorithm>

namespace acc {

static SDL_Vertex Vertex_ConvertToSDL(Vertex vertex);

Texture::Texture(Context *context, std::string filename, Canvas* canvas){
	setName(filename);

	texture = SDL_CreateTextureFromSurface(
			context->getRenderer(),
			canvas->getSurface()
			);

	texture_width = cell_width = canvas->getWidth();
	texture_height = cell_height = canvas->getHeight();
}

Texture::Texture(Context *context, std::string filename, int cell_width, int cell_height){
	setName(filename);

	SDL_Surface *surface = IMG_Load(filename.c_str());

	if(surface == NULL){
		fprintf(stderr, "Failed to load texture: %s\n", filename.c_str());
		texture = NULL;
	}
	else{
		texture = SDL_CreateTextureFromSurface(context->getRenderer(), surface);

		texture_width = surface->w;
		texture_height = surface->h;

		this->cell_width = cell_width;
		this->cell_height = cell_height;

		SDL_FreeSurface(surface);
	}

}

Texture::Texture(Context *context, std::string filename){
	setName(filename);

	SDL_Surface *surface = IMG_Load(filename.c_str());

	if(surface == NULL){
		fprintf(stderr, "Failed to load texture: %s\n", filename.c_str());
		texture = NULL;
	}
	else{
		texture = SDL_CreateTextureFromSurface(context->getRenderer(), surface);

		texture_width = surface->w;
		texture_height = surface->h;

		cell_width = surface->w;
		cell_height = surface->h;

		SDL_FreeSurface(surface);
	}
}

Texture::Texture(Context *context, int dev_texture){
	texture_width = 64;
	texture_height = 64;
	cell_width = 64;
	cell_height = 64;

	SDL_Surface *surface = SDL_CreateRGBSurface(0, texture_width, texture_height, 32, 0, 0, 0, 0);
	uint32_t *pixels = (uint32_t *) surface->pixels;

	switch(dev_texture){
		case DEV_TEXTURE_WHITE:
			setName("dev_texture_white");

			SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 255, 255, 255, 255));
			break;

		case DEV_TEXTURE_XOR:
		default:
			setName("dev_texture_xor");

			for(int i = 0; i < texture_width; i++){
				for(int j = 0; j < texture_height; j++){
					int c = (i ^ j);
					pixels[i + j * texture_width] = 
						SDL_MapRGBA(
								surface->format,
								c * 160 / 64,
								c * 32 / 64,
								c * 240 / 64,
								255
								);
				}
			}
				
			break;
	}

	texture = SDL_CreateTextureFromSurface(context->getRenderer(), surface);

	SDL_FreeSurface(surface);
}

Texture::Texture(Context *context, Font *font, std::string text, uint8_t *color){
	if(text == ""){
		texture = NULL;
		return;
	}

	SDL_Color fg = {color[0], color[1], color[2], color[3]};

	SDL_Surface *surface = TTF_RenderUTF8_Blended(font->getFont(), text.c_str(), fg);

	if(surface == NULL){
		texture = NULL;
		return;
	}

	texture = SDL_CreateTextureFromSurface(context->getRenderer(), surface);
	
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
			context->getRenderer(),
			canvas->getSurface()
			);
}

void Texture::renderCell(Context *context, int x, int y, int id){
	SDL_Rect src_rect = getIdRect(id);
	SDL_Rect dst_rect = getDstRect(x, y, id);

	SDL_RenderCopy(context->getRenderer(), texture, &src_rect, &dst_rect);
}

void Texture::renderCellEx(Context *context, int x, int y, int id, float scale_x, float scale_y, int center_x, int center_y, float angle){
	if(context == NULL || texture == NULL) return;

	SDL_Rect src_rect = getIdRect(id);
	SDL_Rect dst_rect = getDstRect(x, y, id);

	dst_rect.w *= scale_x;
	dst_rect.h *= scale_y;

	center_x *= scale_x;
	center_y *= scale_y;

	SDL_Point center_sdl = {center_x, center_y};

	SDL_RenderCopyEx(
			context->getRenderer(),
			texture,
			&src_rect,
			&dst_rect,
			angle,
			&center_sdl,
			SDL_FLIP_NONE
			);
}

void Texture::renderRect(Context *context, int src[], int dst[]){
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
		SDL_RenderCopy(context->getRenderer(), texture, &src_rect, NULL);
	}
	else{
		dst_rect.x = dst[0];
		dst_rect.y = dst[1];
		dst_rect.w = dst[2];
		dst_rect.h = dst[3];
		SDL_RenderCopy(context->getRenderer(), texture, &src_rect, &dst_rect);
	}

}

void Texture::renderTriangle(Context *context, const Triangle& triangle){
	std::vector<SDL_Vertex> sdl_vertices_vector;
	sdl_vertices_vector.reserve(3);

	std::transform(
			triangle.vertices.begin(),
			triangle.vertices.end(),
			std::back_inserter(sdl_vertices_vector),
			Vertex_ConvertToSDL
			);

	SDL_RenderGeometry(
			context->getRenderer(),
			texture,
			sdl_vertices_vector.data(),
			sdl_vertices_vector.size(),
			NULL,
			0
			);
}

void Texture::renderMesh(Context *context, const Mesh& mesh){
	for(const Triangle& i : mesh.triangles){
		renderTriangle(context, i);
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
