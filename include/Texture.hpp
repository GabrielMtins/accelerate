#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "BaseTexture.hpp"
#include "Resource.hpp"
#include "Context.hpp"
#include "Font.hpp"
#include "Canvas.hpp"

namespace acc {

/**
 * This class manages the creation, loading and rendering of textures.
 */
class Texture : public BaseTexture {
	public:
		/**
		 * Creates a texture with a given Canvas.
		 */
		Texture(Context *context, const std::string& filename, Canvas *canvas);
		/**
		 * Loads a texture from a given file name, cell width and cell height.
		 */
		Texture(Context *context, const std::string& filename, int cell_width, int cell_height);
		/**
		 * Creates a texture from a given string and font.
		 */
		Texture(Context *context, Font *font, const std::string& text, const Color& color, bool anti_aliasing);

		/**
		 * Use this if you created a texture from a canvas and wants to update the
		 * texture with the new information from the canvas. Software renderers
		 * can use this.
		 */
		void updateCanvas(Context *context, Canvas *canvas);

		/**
		 * Renders the cell of a sprite sheet.
		 */
		void renderCell(Context *context, int x, int y, int id);
		/**
		 * Renders the cell of a sprite sheet, but with more
		 * parameters.
		 */
		void renderCellEx(Context *context, int x, int y, int id, float scale_x, float scale_y, int center_x, int center_y, float angle, bool flip_x, bool flip_y);

		int getTextureWidth(void);
		int getTextureHeight(void);
		int getCellWidth(void);
		int getCellHeight(void);

		~Texture(void);
	
	private:
		SDL_Rect getIdRect(int id);
		SDL_Rect getDstRect(int x, int y, int id);

		SDL_Renderer *renderer;
		SDL_Texture *texture;
		int texture_width, texture_height;
		int cell_width, cell_height;
};

};

#endif
