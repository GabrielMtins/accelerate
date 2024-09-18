#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Resource.hpp"
#include "Context.hpp"

namespace acc {

class Texture : Resource{
	public:
		Texture(Context *context, std::string filename, int cell_width, int cell_height);
		Texture(Context *context, std::string filename);

		void renderCell(Context *context, int x, int y, int id);
		void renderRect(Context *context, int src[], int dst[]);

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
