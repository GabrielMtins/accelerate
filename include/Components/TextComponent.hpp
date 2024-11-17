#ifndef TEXT_COMPONENT_HPP
#define TEXT_COMPONENT_HPP

#include "Texture.hpp"
#include "Context.hpp"
#include "Font.hpp"
#include "Color.hpp"

#include <memory>

namespace acc {

struct TextComponent {
	Font *font;

	std::shared_ptr<BaseTexture> texture;

	std::string text;
	std::string old_text;
	
	Color color;

	bool anti_aliasing;

	TextComponent(void);
	TextComponent(Font *font);
	TextComponent(Resource *font);
	bool updateTexture(Context *context);
};

};

#endif
