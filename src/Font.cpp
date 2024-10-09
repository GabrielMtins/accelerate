#include "Font.hpp"

#include <cstdio>

namespace acc {

Font::Font(const std::string& filename, int size){
	font = TTF_OpenFont(filename.c_str(), size);
	setName(filename);

	if(font == NULL){
		fprintf(stderr, "Failed to open font: %s\n", filename.c_str());
	}
}

void Font::setSize(int size){
	if(font != NULL)
		TTF_SetFontSize(font, size);
}

TTF_Font * Font::getFont(void){
	return font;
}

Font::~Font(void){
	if(font != NULL)
		TTF_CloseFont(font);
}

}
