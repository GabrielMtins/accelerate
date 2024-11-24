#include "Scene3d.hpp"
#include "Components/Components.hpp"
#include "Components/Sprite3d.hpp"
#include "Components/Object3dComponent.hpp"
#include "Components/Body3dComponent.hpp"
#include "Components/World3dComponent.hpp"
#include "Components/MovBrush3dComponent.hpp"
#include "Systems/Render3d.hpp"
#include "Systems/Physics3d.hpp"
#include "BrushBuilder.hpp"
#include "Game.hpp"

#include "glad/glad.h"

static const float pi = 3.141592;
static const float epsilon = 0.0f;
static acc::Entity mini_sprite;
static acc::BrushBuilder *builder;
acc::BrushBuilder *other_builder;
acc::Entity movable;

class PlayerBehavior : public acc::BehaviorFunction {
	public:
		void onCreate(void){
			angle = angle_y = 0.0f;
		}

		void onUpdate(void){
			auto& movable_transform = getComponent<acc::TransformComponent>(movable);
			movable_transform.position.z = -1.0f;
			/*
			movable_transform.position.y = 3.0f;
			*/
			movable_transform.position.y = 5.0f + 2.0f * sinf(0.002f * getTicks());

			auto& body = getComponent<acc::Body3dComponent>();

			acc::Vec3 direction;
			acc::Vec3 mouse_motion = getMouseMotion() * getDeltaTime() * 0.5f;

			angle += mouse_motion.x;
			angle_y += mouse_motion.y;

			if(angle_y < -pi/2)
				angle_y = -pi/2;

			if(angle_y > pi/2)
				angle_y = pi/2;

			if(getKey("a")){
				direction.x--;
			}

			if(getKey("d")){
				direction.x++;
			}

			if(getKey("w")){
				direction.z--;
			}

			if(getKey("s")){
				direction.z++;
			}

			if(getKeyDown("p")){
				body.position.print();
			}

			acc::Vec3 new_velocity = direction.normalize().rotateY(-angle) * 8.0f;
			body.velocity.x = new_velocity.x;
			body.velocity.z = new_velocity.z;

			if(getKey("lshift"))
				body.velocity.y = -8.0f;

			if(getKeyDown("space"))
				body.velocity.y = 8.0f;
			
			auto scene = (acc::Scene3d *) getScene();
			acc::Vec3 my_position = getComponent<acc::TransformComponent>().position;

			scene->setViewMatrix(acc::Mat4::RotateX(angle_y) * acc::Mat4::RotateY(angle) * acc::Mat4::Transform(my_position * (-1.0f)));

			auto& sprite = getComponent<acc::Sprite3dComponent>(mini_sprite);
			sprite.rotation = acc::Mat4::RotateY(-angle);
			sprite.id = (getTicks() / 200) % 2;
		}

	private:
		float angle;
		float angle_y;
};

namespace acc {

Scene3d::Scene3d(Game *game) : Scene2d(game){
	registerComponent<Sprite3dComponent>();
	registerComponent<Object3dComponent>();
	registerComponent<Body3dComponent>();
	registerComponent<World3dComponent>();
	registerComponent<MovBrush3dComponent>();

	BrushBuilder *movable_builder;
	
	system_manager->addSystemFront((System *) new Render3dSystem(game->getContext(), &view));
	system_manager->addSystem((System *) new Physics3dSystem((Scene *) this));

	view = Mat4::Identity();

	movable_builder = new BrushBuilder();

	auto texture = (TextureGL *) game->getResource("wall.png");
	movable_builder->setBottomProperties((TextureGL *) game->getResource("floor.png"), -0.4f, Vec3(), Vec3(1.0f, 1.0f, 1.0f));
	movable_builder->setGeometry({Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 0.0f, 0.0f), Vec3(2.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f)});
	movable_builder->setTopProperties((TextureGL *) game->getResource("floor.png"), 0.5f, Vec3(), Vec3(1.0f, 1.0f));
	movable_builder->setWallProperties(texture, Vec3(), Vec3(1.0f, 1.0f));
	movable_builder->build();

	builder = new BrushBuilder();

	builder->setBottomProperties((TextureGL *) game->getResource("floor.png"), -4.5f, Vec3(), Vec3(1.0f, 1.0f, 2.0f));
	builder->setGeometry({Vec3(0.0f, 0.0f, 0.0f), Vec3(10.0f, 0.0f, 0.0f), Vec3(10.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 10.0f)});
	builder->setTopProperties((TextureGL *) game->getResource("floor.png"), -0.5f, Vec3(), Vec3(1.0f, 1.0f));
	builder->setWallProperties(texture, Vec3(), Vec3(1.0f, 1.0f));
	builder->setTopRamp(0.20f, 2);
	builder->build();

	other_builder = new BrushBuilder();

	other_builder->setBottomProperties((TextureGL *) game->getResource("floor.png"), -4.5f, Vec3(), Vec3(1.0f, 1.0f, 2.0f));
	other_builder->setGeometry({Vec3(0.0f, 0.0f, 0.0f), Vec3(10.0f, 0.0f, 0.0f), Vec3(10.0f, 0.0f, -10.0f), Vec3(0.0f, 0.0f, -10.0f)});
	other_builder->setTopProperties((TextureGL *) game->getResource("floor.png"), 2.50f, Vec3(), Vec3(1.0f, 1.0f));
	other_builder->setWallProperties(texture, Vec3(), Vec3(1.0f, 1.0f));
	other_builder->build();

	auto other_shader = (Shader *) game->getResource("other_shader");
	other_shader->setUniform("fog_start", 0.5f);
	other_shader->setUniform("fog_end", 16.0f);
	other_shader->setUniform("fog_color", 0.06f, 0.06f, 0.06f, 1.0f);

	auto my_shader = (Shader *) game->getResource("my_shader");
	my_shader->setUniform("fog_start", 0.5f);
	my_shader->setUniform("fog_end", 16.0f);
	my_shader->setUniform("fog_color", 0.06f, 0.06f, 0.06f, 1.0f);

	{
		Entity next = getNextEntity();
		addComponent<TransformComponent>(next);
		addComponent<Sprite3dComponent>(next, game->getResource("my_shader"), game->getResource("character.png"));

		auto& sprite = getComponent<Sprite3dComponent>(next);

		sprite.rotation = Mat4::RotateY(pi / 2);
		sprite.scale = Mat4::Scale(1.0f, 1.0f, 1.0f);
		sprite.id = 1;

		mini_sprite = next;

		addComponent<Body3dComponent>(next);
		auto& body = getComponent<Body3dComponent>(next);

		body.position = acc::Vec3(8.5, 6.0f, -3.5f);
		body.offset_from_transform = Vec3(0.0f, -0.5f, 0.0f);
		body.gravity = Vec3(0.0f, -10.0f, 0.0f);
		body.setOnCollisionMask(1, true);
		body.setOnCollisionMask(2, true);

		body.cylinder.height = 2.0f;
		body.cylinder.radius = 0.5f;
		body.cylinder.tolerance = 0.0f;
	}

	{
		Entity next = getNextEntity();
		addComponent<TransformComponent>(next);
		addComponent<Sprite3dComponent>(next, game->getResource("my_shader"), game->getResource("character.png"));

		auto& sprite = getComponent<Sprite3dComponent>(next);

		sprite.rotation = Mat4::RotateY(0.0f);
		sprite.scale = Mat4::Scale(1.0f, 1.0f, 1.0f);
		sprite.id = 1;

		mini_sprite = next;

		addComponent<Body3dComponent>(next);
		auto& body = getComponent<Body3dComponent>(next);

		body.position = acc::Vec3(3.5, 6.0f, -3.5f);
		body.offset_from_transform = Vec3(0.0f, -0.5f, 0.0f);
		body.gravity = Vec3(0.0f, -10.0f, 0.0f);
		body.setOnCollisionMask(1, true);
		body.setOnCollisionMask(2, true);

		body.cylinder.height = 2.0f;
		body.cylinder.radius = 0.5f;
		body.cylinder.tolerance = 0.0f;
	}

	{
		Entity next = getNextEntity();
		addComponent<TransformComponent>(next);
		addComponent<Body3dComponent>(next);
		auto ptr = std::make_shared<PlayerBehavior>();
		addComponent<BehaviorComponent>(next, ptr);
		auto& body = getComponent<Body3dComponent>(next);

		body.offset_from_transform = Vec3(0.0f, -1.0f, 0.0f);
		body.position = Vec3(0.0f, 10.0f, 0.0f);
		body.gravity = Vec3(0.0f, -10.0f, 0.0f);
		body.setOnCollisionMask(1, true);
		body.setOnCollisionLayer(2, true);

		body.cylinder.height = 2.0f;
		body.cylinder.radius = 0.5f;
		body.cylinder.tolerance = 0.0f;
	}

	{
		Entity next = getNextEntity();
		addComponent<World3dComponent>(next, game->getResource("other_shader"));

		auto& world = getComponent<World3dComponent>(next);
		world.brushes.push_back(builder);
		world.brushes.push_back(other_builder);
		world.setCollisionLayer(1);

		/*
		world.octree = new Octree(50, 4, Vec3(-32.0f, -32.0f, -32.0f), Vec3(64.0f, 64.0f, 64.0f));
		world.octree->build(world.brushes);

		world.debug_render_octree = true;
		world.octree_shader = (Shader *) game->getResource("cube_shader");
		*/
	}

	{
		Entity next = getNextEntity();
		addComponent<TransformComponent>(next);
		addComponent<MovBrush3dComponent>(next, movable_builder, game->getResource("other_shader"));

		auto& transform = getComponent<acc::TransformComponent>(next);
		transform.position.y = 5.0f;

		auto& mov = getComponent<MovBrush3dComponent>(next);
		mov.setCollisionLayer(1);
		
		movable = next;
	}
}

void Scene3d::setViewMatrix(const Mat4& view){
	this->view = view;
}

};
