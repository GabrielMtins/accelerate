#include "Texture.hpp"

#include <algorithm>

namespace acc {

static SDL_Vertex Vertex_ConvertToSDL(Vertex vertex);

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

void Texture::renderMesh(Context *context, Mesh &mesh){
	auto& vertices_vector = mesh.getVertices();
	auto& indices_vector = mesh.getIndices();

	std::vector<SDL_Vertex> sdl_vertices_vector;
	sdl_vertices_vector.reserve(vertices_vector.size());

	std::transform(
			vertices_vector.begin(),
			vertices_vector.end(),
			std::back_inserter(sdl_vertices_vector),
			Vertex_ConvertToSDL
			);

	if(indices_vector.size() != 0){
		SDL_RenderGeometry(
				context->getRenderer(),
				texture,
				sdl_vertices_vector.data(),
				sdl_vertices_vector.size(),
				indices_vector.data(),
				indices_vector.size()
				);
	}
	else{
		SDL_RenderGeometry(
				context->getRenderer(),
				texture,
				sdl_vertices_vector.data(),
				sdl_vertices_vector.size(),
				NULL,
				0
				);
	}
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

static SDL_Vertex Vertex_ConvertToSDL(Vertex vertex){
	vertex.color *= 255.0f;

	SDL_Vertex transformed = {
		{vertex.position.x, vertex.position.y},
		{(uint8_t) vertex.color.x, (uint8_t) vertex.color.y, (uint8_t) vertex.color.z, vertex.alpha},
		{vertex.uv.x, vertex.uv.y}
	};

	return transformed;
}

};
