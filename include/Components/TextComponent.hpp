#ifndef TEXT_COMPONENT_HPP
#define TEXT_COMPONENT_HPP

#include "Texture.hpp"
#include "Context.hpp"
#include "Font.hpp"

#include <memory>

namespace acc {

struct TextComponent {
	Font *font;

	std::shared_ptr<Texture> texture;

	std::string text;
	std::string old_text;
	
	uint8_t color[4];

	TextComponent(void);
	TextComponent(Font *font);
	bool updateTexture(Context *context);
};

};

#endif
