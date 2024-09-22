#include "Systems/TextRender.hpp"
#include "Components/Components.hpp"

namespace acc {

TextRender::TextRender(Scene *scene){
	context = scene->getGame()->getContext();
}

void TextRender::update(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<TextComponent>())
		return;

	auto arr = component_manager->getComponentArray<TextComponent>();

	for(size_t i = 0; i < arr->getSize(); i++){
		Entity entity = arr->indexToEntity(i);

		if(component_manager->hasComponent<SpriteComponent>(entity))
			continue;

		auto& text_component = arr->atIndex(i);
		auto& sprite_component = component_manager->getComponent<SpriteComponent>(entity);

		if(text_component.updateTexture(context)){
			sprite_component.texture = text_component.texture.get();
		}
	}
}

};
