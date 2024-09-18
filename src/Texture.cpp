#include "Texture.hpp"

namespace acc {

Texture::Texture(Context *context, std::string filename, int cell_width, int cell_height){
	SDL_Surface *surface = IMG_Load(filename.c_str());

	if(surface == NULL){
		fprintf(stderr, "Failed to load texture: %s\n", filename.c_str());
	}

	setName(filename);

	texture = SDL_CreateTextureFromSurface(context->getRenderer(), surface);

	texture_width = surface->w;
	texture_height = surface->h;

	this->cell_width = cell_width;
	this->cell_height = cell_height;

	SDL_FreeSurface(surface);
}

Texture::Texture(Context *context, std::string filename){
	SDL_Surface *surface = IMG_Load(filename.c_str());

	if(surface == NULL){
		fprintf(stderr, "Failed to load texture: %s\n", filename.c_str());
	}

	setName(filename);

	texture = SDL_CreateTextureFromSurface(context->getRenderer(), surface);

	texture_width = surface->w;
	texture_height = surface->h;

	cell_width = surface->w;
	cell_height = surface->h;

	SDL_FreeSurface(surface);
}

void Texture::renderCell(Context *context, int x, int y, int id){
	if(context == NULL || texture == NULL) return;

	SDL_Rect dst_rect = {
		x, y, cell_width, cell_height
	};

	if(id == -1){
		dst_rect.w = texture_width;
		dst_rect.h = texture_height;
		SDL_RenderCopy(context->getRenderer(), texture, NULL, &dst_rect);
		return;
	}
	else{
		int num_tex_x = texture_width / cell_width;

		int id_x = (id % num_tex_x);
		int id_y = (id / num_tex_x);

		SDL_Rect src_rect = {
			id_x * cell_width,
			id_y * cell_height,
			cell_width,
			cell_height
		};

		SDL_RenderCopy(context->getRenderer(), texture, &src_rect, &dst_rect);
	}
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

int Texture::getCellWidth(void){
	return cell_width;
}

int Texture::getCellHeight(void){
	return cell_height;
}

Texture::~Texture(void){
	SDL_DestroyTexture(texture);
}

};