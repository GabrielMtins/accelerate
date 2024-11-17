#include "Components/TextComponent.hpp"

namespace acc {

TextComponent::TextComponent(void){
	font = NULL;
	texture = nullptr;
	text = "";
	old_text = "";
	anti_aliasing = false;

	color = Color(0xff, 0xff, 0xff, 0xff);
}

TextComponent::TextComponent(Font *font){
	this->font = font;
	texture = nullptr;
	text = "";
	old_text = "";
	anti_aliasing = false;

	color = Color(0xff, 0xff, 0xff, 0xff);
}

TextComponent::TextComponent(Resource *font){
	this->font = (Font *) font;
	texture = nullptr;
	text = "";
	old_text = "";
	anti_aliasing = false;

	color = Color(0xff, 0xff, 0xff, 0xff);
}

bool TextComponent::updateTexture(Context *context){
	if(old_text == text) return false;

	old_text = text;

	if(text == ""){
		texture = nullptr;
	}
	else{
		texture.reset(context->getRenderer()->loadTexture(context, font, text, color, anti_aliasing));
		//texture = std::make_shared<Texture>(context, font, text, color, anti_aliasing);
	}

	return true;
}

};
