#include "Scene2d.hpp"
#include "Game.hpp"

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

			auto& body = getComponent<BodyComponent>();

			body.size = Vec3(32, 32, 0);
		}

		void onUpdate(void){
			Scene2d *scene = (Scene2d *) getScene();
			auto& body = getComponent<BodyComponent>();
			auto& sprite = getComponent<SpriteComponent>();

			const uint8_t *keys = SDL_GetKeyboardState(NULL);

			dir_vel = Vec3();

			//scene->setCameraPosition(body.position - Vec3(200, 100));

			if(keys[SDL_SCANCODE_J]){
				was_pressed = true;
			}
			else if(was_pressed){
				getGame()->setScene((Scene *) new Scene2d(getGame()));
			}

			if(keys[SDL_SCANCODE_D]) dir_vel.x++;
			if(keys[SDL_SCANCODE_A]) dir_vel.x--;
			if(keys[SDL_SCANCODE_S]) dir_vel.y++;
			if(keys[SDL_SCANCODE_W]) dir_vel.y--;

			//body.velocity = dir_vel.normalize() * 60;
			body.velocity.x = dir_vel.x * 40;

			if(dir_vel.y != 0)
				body.velocity.y = dir_vel.y * 100;

			timer += getContext()->getDeltaTime();

			if(timer > 0.1){
				timer = 0;
				sprite.id++;
				sprite.id %= 2;
			}

			if(dir_vel.lengthSqr() == 0)
				sprite.id = 0;
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

	system_manager->addSystem(
			(System *) new Render2dSystem(game->getContext(), &camera_position)
			);

	system_manager->addSystem(
			(System *) new BehaviorSystem(this)
			);

	system_manager->addSystem(
			(System *) new PhysicsSystem(this)
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

		behavior.setFunction((BehaviorFunction *) new TestBehavior());

		transform.position.x = 32;
		sprite.id = 0;

		body.position = Vec3(32, 0, 0);
		body.size = Vec3(64, 64, 0);
		body.setOnCollisionMask(1, true);
		body.gravity = Vec3(0, 100, 0);
	}

	/*
	{
		Entity next_entity = getNextEntity();
	
		addComponent<TransformComponent>(next_entity);
		addComponent<SpriteComponent>(next_entity, SpriteComponent((Texture *) game->getResource("player.png")));
		addComponent<BodyComponent>(next_entity);

		auto& sprite = getComponent<SpriteComponent>(next_entity);
		auto& body = getComponent<BodyComponent>(next_entity);

		body.position = Vec3(32, 128, 32);
		body.size = Vec3(32, 32, 0);
		body.setOnCollisionLayer(1, true);
		sprite.id = 0;
	}
	*/

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
			tile.setTile(i, 4, 0);
		}
		//tile.setTile(3, 5, 0);
		//tile.setTile(3, 6, 0);
		tile.setCollisionLayer(1);
	}
}

void Scene2d::setCameraPosition(Vec3 camera_position){
	this->camera_position = camera_position;
}

};
