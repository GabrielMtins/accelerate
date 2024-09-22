#ifndef FONT_HPP
#define FONT_HPP

#include "Resource.hpp"
#include "Context.hpp"

namespace acc {

class Font : public Resource {
	public:
		Font(std::string filename, int size);
		void setSize(int size);
		TTF_Font * getFont(void);
		~Font(void);
	
	private:
		TTF_Font *font;
};

};

#endif
