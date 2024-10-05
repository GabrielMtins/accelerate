#ifndef FONT_HPP
#define FONT_HPP

#include "Resource.hpp"
#include "Context.hpp"

namespace acc {

class Font : public Resource {
	public:
		/**
		 * Reads a font from the file with filepath "filename" and a given size.
		 */
		Font(std::string filename, int size);
		/**
		 * Resizes the size of the font
		 */
		void setSize(int size);
		/**
		 * Returns the pointer for the SDL font struct
		 */
		TTF_Font * getFont(void);
		~Font(void);
	
	private:
		TTF_Font *font;
};

};

#endif
