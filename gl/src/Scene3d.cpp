#include "Scene3d.hpp"
#include "Components/Components.hpp"
#include "Components/Sprite3d.hpp"
#include "Systems/Render3d.hpp"
#include "Game.hpp"

class PlayerBehavior : public acc::BehaviorFunction {
	public:
		void onCreate(void){
			angle = 0.0f;
		}

		void onUpdate(void){
			auto& body = getComponent<acc::BodyComponent>();

			if(getKey("a"))
				angle -= getDeltaTime() * 2.0f;

			if(getKey("d"))
				angle += getDeltaTime() * 2.0f;

			if(getKey("w")){
				body.velocity = acc::Vec3(0.0f, 0.0f, -5.0f).rotateY(-angle);
			}
			else if(getKey("s")){
				body.velocity = acc::Vec3(0.0f, 0.0f, 5.0f).rotateY(-angle);
			}
			else{
				body.velocity = acc::Vec3();
			}
			
			auto scene = (acc::Scene3d *) getScene();

			scene->setViewMatrix(acc::Mat4::RotateY(angle) * acc::Mat4::Transform(body.position * (-1.0f)));
		}

	private:
		float angle;
};

namespace acc {

Scene3d::Scene3d(Game *game) : Scene2d(game){
	registerComponent<Sprite3dComponent>();
	system_manager->addSystemFront((System *) new Render3dSystem(game->getContext(), &view));
	view = Mat4::Identity();

	{
		Entity next = getNextEntity();
		addComponent<TransformComponent>(next);
		addComponent<Sprite3dComponent>(next, game->getResource("my_shader"), game->getResource("wall.png"));

		auto& transform = getComponent<TransformComponent>(next);
		auto& sprite = getComponent<Sprite3dComponent>(next);

		transform.position.z = -3.0f;
		sprite.rotation = Mat4::RotateY(0.9f);
		sprite.scale = Mat4::Scale(5.0f, 1.0f, 1.0f);
	}

	{
		Entity next = getNextEntity();
		addComponent<TransformComponent>(next);
		addComponent<BodyComponent>(next);
		auto ptr = std::make_shared<PlayerBehavior>();
		addComponent<BehaviorComponent>(next, ptr);
	}
}

void Scene3d::setViewMatrix(const Mat4& view){
	this->view = view;
}

};
