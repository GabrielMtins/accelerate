#include "Scene2d.hpp"
#include "Game.hpp"
#include "Font.hpp"

#include "Components/Components.hpp"
#include "Systems/Systems.hpp"

namespace acc {

Scene2d::Scene2d(Game *game) : Scene(game){
	registerComponent<TransformComponent>();
	registerComponent<SpriteComponent>();
	registerComponent<DrawRectComponent>();
	registerComponent<BehaviorComponent>();
	registerComponent<LabelComponent>();
	registerComponent<BodyComponent>();
	registerComponent<TilesetComponent>();
	registerComponent<TextComponent>();

	system_manager->addSystem(
			(System *) new PhysicsSystem(this)
			);

	system_manager->addSystem(
			(System *) new BehaviorSystem(this)
			);

	system_manager->addSystem(
			(System *) new Render2dSystem(game->getContext(), &camera_position)
			);
}

void Scene2d::setCameraPosition(Vec3 camera_position){
	this->camera_position = camera_position;
}

Vec3 Scene2d::getCameraPosition(void){
	return camera_position;
}

};
