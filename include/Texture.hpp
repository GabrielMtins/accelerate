#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Resource.hpp"
#include "Context.hpp"
#include "Mesh.hpp"

namespace acc {

enum DEV_TEXTURE_TYPE {
	DEV_TEXTURE_WHITE,
	DEV_TEXTURE_XOR
};

class Texture : Resource{
	public:
		Texture(Context *context, std::string filename, int cell_width, int cell_height);
		Texture(Context *context, std::string filename);
		Texture(Context *context, int dev_texture);

		void renderCell(Context *context, int x, int y, int id);
		void renderRect(Context *context, int src[], int dst[]);

		void renderTriangle(Context *context, Triangle &triangle);

		void renderMesh(Context *context, Mesh &mesh);

		int getTextureWidth(void);
		int getTextureHeight(void);
		int getCellWidth(void);
		int getCellHeight(void);

		~Texture(void);
	
	private:
		SDL_Texture *texture;
		int texture_width, texture_height;
		int cell_width, cell_height;
};

};

#endif
