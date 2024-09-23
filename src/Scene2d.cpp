#include "Scene2d.hpp"
#include "Game.hpp"
#include "Font.hpp"

#include "Components/Components.hpp"
#include "Systems/Systems.hpp"

namespace acc {

class TestBehavior : public BehaviorFunction {
	public:
		void onCreate(void){
			was_pressed = false;
			printf("Hello World!\n");
			timer = 0;

			std::vector<Entity> found = find<BodyComponent>([](BodyComponent& a){return a.gravity.length() == 0;});

			for(auto i : found){
				auto& sprite = getComponent<SpriteComponent>(i);
				auto& body = getComponent<BodyComponent>(i);

				body.size = Vec3(196, 64, 0);
				sprite.id = -1;
			}
		}

		void onUpdate(void){
			auto scene = (Scene2d *) getScene();
			auto& body = getComponent<BodyComponent>();
			auto& sprite = getComponent<SpriteComponent>();

			dir_vel = Vec3();

			if(getMouseButtonUp("left"))
				printf("hello\n");

			//scene->setCameraPosition(body.position - Vec3(200, 100));

			if(getKeyUp("j")){
				printf("teste\n");
				//destroy();
			}

			if(getKeyDown("a")) body.velocity.x = -50;
			if(getKeyDown("d")) body.velocity.x = +50;
			if(getKeyDown("w")) body.velocity.y = -75;
			if(getKeyDown("s")) body.velocity.y = +50;

			timer += getContext()->getDeltaTime();

			if(timer > 0.1){
				timer = 0;
				sprite.id++;
				sprite.id %= 2;
			}

			if(dir_vel.lengthSqr() == 0)
				sprite.id = 0;

			body.position = getMousePosition();
		}

		void onCollision(Entity other){
		}

	private:
		float timer;
		Vec3 dir_vel;
		bool was_pressed;
};

Scene2d::Scene2d(Game *game) : Scene(game){
	registerComponent<TransformComponent>();
	registerComponent<SpriteComponent>();
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

	{
		Entity next_entity = getNextEntity();
	
		addComponent<TransformComponent>(next_entity);
		addComponent<SpriteComponent>(next_entity, SpriteComponent((Texture *) game->getResource("player.png")));
		addComponent<BehaviorComponent>(next_entity);
		addComponent<BodyComponent>(next_entity);

		auto& sprite = getComponent<SpriteComponent>(next_entity);
		auto& transform = getComponent<TransformComponent>(next_entity);
		auto& behavior = getComponent<BehaviorComponent>(next_entity);
		auto& body = getComponent<BodyComponent>(next_entity);

		behavior.setFunction((std::make_shared<TestBehavior>()));

		transform.position.x = 32;
		sprite.id = 0;

		body.position = Vec3(32, 32, 0);
		body.offset_from_transform = Vec3(12, 0, 0);
		body.size = Vec3(10, 32, 0);
		body.setOnCollisionMask(1, true);
		//body.gravity = Vec3(0, 100, 0);
	}

	{
		Entity next_entity = getNextEntity();
	
		addComponent<TransformComponent>(next_entity);
		addComponent<SpriteComponent>(next_entity, SpriteComponent(game->getResource("player.png")));
		addComponent<TextComponent>(next_entity, TextComponent(game->getResource("default.ttf")));

		auto& transform = getComponent<TransformComponent>(next_entity);
		auto& text_component = getComponent<TextComponent>(next_entity);
		auto& sprite = getComponent<SpriteComponent>(next_entity);

		text_component.text = "oi";
		transform.position.x = 32;
		sprite.follow_camera = false;
	}

	{
		Entity next_entity = getNextEntity();
	
		addComponent<TilesetComponent>(next_entity,
				TilesetComponent(
					(Texture *) game->getResource("texture.png"),
					16,
					16
					)
				);

		auto& tile = getComponent<TilesetComponent>(next_entity);

		for(int i = 0; i < 10; i++){
			tile.setTile(i, 0, 0);
			tile.setTile(i, 6, 0);
		}
		tile.setCollisionLayer(1);
	}
}

void Scene2d::setCameraPosition(Vec3 camera_position){
	this->camera_position = camera_position;
}

};
