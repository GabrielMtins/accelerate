#include "ScenePortal3d.hpp"

#include "Components/Components.hpp"
#include "Components/PortalWorldComponent.hpp"
#include "Systems/RenderPortal3d.hpp"

namespace acc {

ScenePortal3d::ScenePortal3d(Game *game) : Scene2d(game){
	registerComponent<PortalWorldComponent>();

	Context *context = game->getContext();

	framebuffer_canvas = new Canvas(
			"framebuffer_canvas",
			context->getWidth(),
			context->getHeight()
			);

	framebuffer_texture = new Texture(
			context,
			"framebuffer_texture",
			framebuffer_canvas
			);

	system_manager->addSystem((System *) new RenderPortal3d(this));

	{
		Entity entity = getNextEntity();

		addComponent<TransformComponent>(entity);
		addComponent<SpriteComponent>(entity, SpriteComponent(framebuffer_texture));
		addComponent<PortalWorldComponent>(entity);

		auto& sprite_component = getComponent<SpriteComponent>(entity);
		sprite_component.layer = 0;
	}
}

Canvas * ScenePortal3d::getFramebufferCanvas(void){
	return framebuffer_canvas;
}

Texture * ScenePortal3d::getFramebufferTexture(void){
	return framebuffer_texture;
}

ScenePortal3d::~ScenePortal3d(void){
	delete framebuffer_canvas;
	delete framebuffer_texture;
}

};
