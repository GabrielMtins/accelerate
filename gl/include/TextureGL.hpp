#ifndef TEXTURE_GL_HPP
#define TEXTURE_GL_HPP

#include "BaseTexture.hpp"

namespace acc {

class TextureGL : public BaseTexture {
	public:
		TextureGL(Context *context, const std::string& filename, int cell_width, int cell_height);
		TextureGL(Context *context, Font *font, const std::string& text, const Color& color, bool anti_aliasing);

		void renderCell(Context *context, int x, int y, int id);
		void renderCellEx(Context *context, int x, int y, int id, float scale_x, float scale_y, int center_x, int center_y, float angle, bool flip_x, bool flip_y);
		void renderRect(Context *context, int src[], int dst[]);

		int getTextureWidth(void);
		int getTextureHeight(void);
		int getCellWidth(void);
		int getCellHeight(void);

		~TextureGL(void);
	
	private:
		void setId(Context *context);
		bool loaded;
		unsigned int texture_id;
		int texture_width, texture_height, cell_width, cell_height;
};

};

#endif
