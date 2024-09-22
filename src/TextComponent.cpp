#include "Components/TextComponent.hpp"

namespace acc {

TextComponent::TextComponent(Font *font){
	this->font = font;
	texture = nullptr;
	text = "";
	old_text = "";

	for(int i = 0; i < 4; i++)
		color[i] = 0xff;
}

bool TextComponent::updateTexture(Context *context){
	if(old_text == text) return false;

	old_text = text;

	if(text == ""){
		texture = nullptr;
	}
	else{
		texture = std::make_shared<Texture>(context, font, text, color);
	}

	return true;
}

};
