#include "acc.hpp"
#include "acc_gl.hpp"

acc::TextureGL *texture;
acc::Shader *my_shader;
acc::Mesh *my_mesh;

class MyGame : public acc::Game {
	public:
		MyGame(acc::Context *context) : acc::Game(context){
		}

		void update(void){
			getContext()->clearScreen(0x20, 0x20, 0x20, 0xff);

			acc::Mat4 model = acc::Mat4::RotateY(0.001f * getContext()->getTicks());
			acc::Mat4 view = acc::Mat4::Transform(0.0f, 0.0f, -1.8f);
			acc::Mat4 projection = acc::Mat4::PerspectiveProjection((float) getContext()->getWidth() / getContext()->getHeight(), 3.141592 / 2, 100.0f, 0.25f);

			my_shader->setUniform("model", model);
			my_shader->setUniform("view", view);
			my_shader->setUniform("projection", projection);
			my_shader->setTexture("model_texture", texture->getId(), 0);
			my_mesh->render(my_shader);

			acc::RendererGL *gl = (acc::RendererGL *) getContext()->getRenderer();
			gl->setDepthBuffer(false);
			texture->renderCell(getContext(), 0, 0, 0);
			gl->setDepthBuffer(true);

			getContext()->renderPresent();
		}
};

int main(int argc, char **argv){
	acc::RendererGL::init();
	my_shader = new acc::Shader();
	my_mesh = new acc::Mesh();

	acc::Context *context = new acc::Context("test", 1280, 720, SDL_WINDOW_OPENGL);
	context->setRenderer((acc::Renderer *) new acc::RendererGL(context));
	
	context->setScaling(acc::RENDERER_SCALING_BESTFIT);

	texture = new acc::TextureGL(context, "wall.png", 64, 64);

	my_shader->loadFile("vertex.vs", "fragment.fs");
	my_mesh->buildUnitQuad();

	acc::Game *game = (acc::Game *) new acc::Game(context);

	game->loadResource((acc::Resource *) new acc::Shader("my_shader", "shaders/sprite.vs", "shaders/sprite.fs"));
	game->loadResource((acc::Resource *) texture);

	game->setScene((acc::Scene *) new acc::Scene3d(game));

	game->run();

	delete game;
}
