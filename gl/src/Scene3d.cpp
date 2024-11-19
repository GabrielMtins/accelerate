#include "Scene3d.hpp"
#include "Components/Components.hpp"
#include "Components/Sprite3d.hpp"
#include "Components/Brush3dComponent.hpp"
#include "Systems/Render3d.hpp"
#include "BrushBuilder.hpp"
#include "Game.hpp"

static const float pi = 3.141592;
static const float epsilon = 0.0f;

class PlayerBehavior : public acc::BehaviorFunction {
	public:
		void onCreate(void){
			angle = angle_y = 0.0f;
		}

		void onUpdate(void){
			auto& body = getComponent<acc::BodyComponent>();

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

			body.velocity = direction.normalize().rotateY(-angle) * 3.0f;

			if(getKey("lshift"))
				body.velocity.y = -3.0f;

			if(getKey("space"))
				body.velocity.y = 3.0f;
			
			auto scene = (acc::Scene3d *) getScene();

			scene->setViewMatrix(acc::Mat4::RotateX(angle_y) * acc::Mat4::RotateY(angle) * acc::Mat4::Transform(body.position * (-1.0f)));
		}

	private:
		float angle;
		float angle_y;
};

namespace acc {

Scene3d::Scene3d(Game *game) : Scene2d(game){
	registerComponent<Sprite3dComponent>();
	registerComponent<Brush3dComponent>();
	
	system_manager->addSystemFront((System *) new Render3dSystem(game->getContext(), &view));
	view = Mat4::Identity();

	/*
	Mesh *my_mesh = new Mesh();
	my_mesh->buildUnitTetrahedron();

	Model3d *my_model = new Model3d();
	my_model->addMesh(my_mesh, (TextureGL *) game->getResource("wall.png"));
	*/

	BrushBuilder *builder = new BrushBuilder();

	auto texture = (TextureGL *) game->getResource("wall.png");
	builder->setBottomProperties(texture, -0.5f, Vec3(), Vec3(1.0f, 1.0f, 1.0f));
	builder->setGeometry({Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(2.0f, 0.0f, 2.0f), Vec3(0.0f, 0.0f, 1.0f)});
	builder->setTopProperties(texture, 0.5f, Vec3(), Vec3(1.0f, 1.0f, 1.0f));
	builder->setWallProperties(texture, Vec3(), Vec3(1.0f, 1.0f, 1.0f));

	Model3d *my_model = builder->build();

	{
		Entity next = getNextEntity();
		addComponent<TransformComponent>(next);
		addComponent<Sprite3dComponent>(next, game->getResource("my_shader"), game->getResource("character.png"));

		auto& transform = getComponent<TransformComponent>(next);
		auto& sprite = getComponent<Sprite3dComponent>(next);

		transform.position = acc::Vec3(0.5f, 0.625f, 0.5f);
		sprite.rotation = Mat4::RotateY(0.9f);
		sprite.scale = Mat4::Scale(0.25f, 0.25f, 0.25f);
	}

	{
		Entity next = getNextEntity();
		addComponent<TransformComponent>(next);
		addComponent<BodyComponent>(next);
		auto ptr = std::make_shared<PlayerBehavior>();
		addComponent<BehaviorComponent>(next, ptr);
	}

	{
		Entity next = getNextEntity();
		addComponent<Brush3dComponent>(next, my_model, game->getResource("other_shader"));
	}
}

void Scene3d::setViewMatrix(const Mat4& view){
	this->view = view;
}

};
