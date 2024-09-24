#include "TestScene.hpp"
#include "Components/Components.hpp"
#include <memory>
#include <cmath>

using namespace acc;

class TestBehavior : public BehaviorFunction {
	public:
		void onCreate(void){
			was_pressed = false;
			printf("Hello World!\n");
			timer = 0;
		}

		void onUpdate(void){
			auto scene = (Scene2d *) getScene();
			auto& transform = getComponent<BodyComponent>();
			auto& body = getComponent<BodyComponent>();
			auto& sprite = getComponent<SpriteComponent>();

			//sprite.angle = timer * 45;
			//sprite.scale = Vec3(1.0f, 1.0f) * (cos(timer) + 2);
			sprite.scale = Vec3(3.0f, 0.1f);
			sprite.angle = timer * 45;
			sprite.center = Vec3(16.0f, 16.0f);
			//sprite.center = Vec3();

			dir_vel = Vec3();

			if(getMouseButtonUp("left"))
				printf("hello\n");

			scene->setCameraPosition(transform.position - Vec3(200, 100));

			timer += getDeltaTime();

			if(getKeyUp("j")){
				printf("teste\n");
				//destroy();
			}

			sprite.id = 0;

			//body.velocity = Vec3();

			if(getKey("a")) body.velocity.x = -50;
			if(getKey("d")) body.velocity.x = +50;
			if(getKey("w")) body.velocity.y = -75;
			if(getKey("s")) body.velocity.y = +50;
		}

		void onCollision(Entity other){
			if(hasComponent<TilesetComponent>(other)){
				auto& sprite = getComponent<SpriteComponent>();

				sprite.id = 1;
			}
		}

	private:
		float timer;
		Vec3 dir_vel;
		bool was_pressed;
};

TestScene::TestScene(Game *game) : Scene2d(game) {
	{
		Entity next_entity = getNextEntity();
	
		addComponent<TransformComponent>(next_entity);
		addComponent<SpriteComponent>(next_entity, SpriteComponent(game->getResource("test.png")));
		addComponent<BehaviorComponent>(next_entity);
		addComponent<BodyComponent>(next_entity);

		auto& sprite = getComponent<SpriteComponent>(next_entity);
		auto& behavior = getComponent<BehaviorComponent>(next_entity);
		auto& body = getComponent<BodyComponent>(next_entity);

		behavior.setFunction((std::make_shared<TestBehavior>()));

		body.position = Vec3(32, 32, 0);
		body.offset_from_transform = Vec3(0, 0, 0);
		body.size = Vec3(32, 32, 1);
		body.setOnCollisionMask(1, true);
		body.gravity = Vec3(0, 100, 0);

		sprite.id = 0;
		sprite.center = body.size / 2;
		sprite.angle = 36;
	}

	{
		Entity next_entity = getNextEntity();
	
		addComponent<TransformComponent>(next_entity);
		addComponent<SpriteComponent>(next_entity, SpriteComponent(game->getResource("player.png")));
		addComponent<BodyComponent>(next_entity);

		auto& sprite = getComponent<SpriteComponent>(next_entity);
		auto& body = getComponent<BodyComponent>(next_entity);

		body.position = Vec3(64, 80);
		body.offset_from_transform = Vec3(12, 0, 0);
		body.size = Vec3(10, 32);
		sprite.follow_camera = true;
		sprite.id = 2;
		body.setOnCollisionLayer(1, true);
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
			tile.setTile(i + 1, 3, 0);
			tile.setTile(i + 1, 9, 0);
		}
		tile.setCollisionLayer(1);
	}
}
