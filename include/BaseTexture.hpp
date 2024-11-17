#ifndef BASE_TEXTURE_HPP
#define BASE_TEXTURE_HPP

#include "Resource.hpp"
#include "Context.hpp"
#include "Font.hpp"
#include "Canvas.hpp"

namespace acc {

class BaseTexture : public Resource {
	public:
		virtual void renderCell(Context *context, int x, int y, int id) = 0;
		virtual void renderCellEx(Context *context, int x, int y, int id, float scale_x, float scale_y, int center_x, int center_y, float angle, bool flip_x, bool flip_y) = 0;
		virtual void renderRect(Context *context, int src[], int dst[]) = 0;

		virtual int getTextureWidth(void) = 0;
		virtual int getTextureHeight(void) = 0;
		virtual int getCellWidth(void) = 0;
		virtual int getCellHeight(void) = 0;

		virtual ~BaseTexture(void) = default;
};

};

#endif
