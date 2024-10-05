#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Resource.hpp"
#include "Context.hpp"
#include "Font.hpp"
#include "Mesh.hpp"
#include "Canvas.hpp"

namespace acc {

enum DEV_TEXTURE_TYPE {
	DEV_TEXTURE_WHITE,
	DEV_TEXTURE_XOR
};

/**
 * This class manages the creation, loading and rendering of textures.
 */
class Texture : Resource{
	public:
		/**
		 * Creates a texture with a given Canvas.
		 */
		Texture(Context *context, std::string filename, Canvas *canvas);
		/**
		 * Loads a texture from a given file name, cell width and cell height.
		 */
		Texture(Context *context, std::string filename, int cell_width, int cell_height);
		/**
		 * Loads a texture from a given file name.
		 */
		Texture(Context *context, std::string filename);
		/**
		 * Creates a custom dev texture.
		 */
		Texture(Context *context, int dev_texture);
		/**
		 * Creates a texture from a given string and font.
		 */
		Texture(Context *context, Font *font, std::string text, const Color& color, bool anti_aliasing);

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
		void renderCellEx(Context *context, int x, int y, int id, float scale_x, float scale_y, int center_x, int center_y, float angle);
		/**
		 * Renders a texture with a given src rect and dst rect.
		 */
		void renderRect(Context *context, int src[], int dst[]);

		void renderTriangle(Context *context, const Triangle& triangle);

		void renderMesh(Context *context, const Mesh& mesh);

		int getTextureWidth(void);
		int getTextureHeight(void);
		int getCellWidth(void);
		int getCellHeight(void);

		~Texture(void);
	
	private:
		SDL_Rect getIdRect(int id);
		SDL_Rect getDstRect(int x, int y, int id);

		SDL_Texture *texture;
		int texture_width, texture_height;
		int cell_width, cell_height;
};

};

#endif
