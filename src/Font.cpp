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

int Font::widthText(const std::string& text){
	int max_w, w;
	std::string tmp;

	max_w = 0;

	for(char c : text){
		if(c == '\n'){
			TTF_SizeText(font, tmp.c_str(), &w, NULL);

			if(w > max_w)
				max_w = w;
			
			tmp = "";
		}
		else{
			tmp += c;
		}
	}

	TTF_SizeText(font, tmp.c_str(), &w, NULL);

	if(w > max_w)
		max_w = w;

	return max_w;
}

Font::~Font(void){
	if(font != NULL)
		TTF_CloseFont(font);
}

}
