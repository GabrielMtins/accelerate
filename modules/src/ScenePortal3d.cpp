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

		auto& world = getComponent<PortalWorldComponent>(entity);

		{
			Sector sector;

			sector.floor_height = -0.5f;
			sector.celling_height = +0.8f;
			//sector.floor_texture.scaling *= 2.0f;
			sector.floor_texture.offset = Vec3(0.25f, 0.25f);

			Wall wall;
			wall.a = Vec3(3.0f, 0.0f, 0.0f);
			wall.b = Vec3(3.0f, 2.0f, 0.0f);
			wall.texture.id = 1;
			wall.texture.scaling = Vec3(1.0f, 1.0f);
			wall.texture.offset = Vec3(0.5f, 0.1f);
			wall.portal = -1;
	
			sector.walls.push_back(wall);

			wall.a = Vec3(2.0f, 3.0f, 0.0f);
			wall.b = Vec3(0.0f, 3.0f, 0.0f);
			wall.texture.offset = Vec3(0.0f, 0.0f);
			wall.texture.scaling = Vec3(1.0f, 1.0f);
	
			sector.walls.push_back(wall);

			wall.a = Vec3(3.0f, 2.0f, 0.0f);
			wall.b = Vec3(2.0f, 3.0f, 0.0f);
			wall.texture.id = 1;
			wall.portal = 1;
	
			sector.walls.push_back(wall);

			world.sectors.push_back(sector);
		}

		{
			Sector sector;

			sector.floor_height = -0.3f;
			sector.celling_height = +0.7f;

			Wall wall;
			wall.a = Vec3(2.0f, 3.0f, 0.0f);
			wall.b = Vec3(2.0f, 4.0f, 0.0f);
			wall.texture.id = 1;
			wall.portal = -1;
	
			sector.walls.push_back(wall);

			wall.a = Vec3(4.0f, 2.0f, 0.0f);
			wall.b = Vec3(2.0f, 4.0f, 0.0f);
			wall.portal = -1;
	
			sector.walls.push_back(wall);

			wall.a = Vec3(4.0f, 2.0f, 0.0f);
			wall.b = Vec3(3.0f, 2.0f, 0.0f);
	
			sector.walls.push_back(wall);

			wall.a = Vec3(3.0f, 2.0f, 0.0f);
			wall.b = Vec3(2.0f, 3.0f, 0.0f);
			wall.portal = 0;
	
			sector.walls.push_back(wall);

			world.sectors.push_back(sector);
		}
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
