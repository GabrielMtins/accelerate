#include "acc.hpp"
#include "acc_gl.hpp"

acc::TextureGL *texture;
acc::Shader *my_shader;
acc::Mesh *my_mesh;

int main(int argc, char **argv){
	acc::RendererGL::init();
	my_shader = new acc::Shader();
	my_mesh = new acc::Mesh();

	acc::Context *context = new acc::Context("test", 640, 360, SDL_WINDOW_OPENGL);
	context->setRenderer((acc::Renderer *) new acc::RendererGL(context));
	context->setRelativeMode(true);
	context->setFullscreenMode();
	
	context->setScaling(acc::RENDERER_SCALING_BESTFIT);
	context->setFps(165);

	texture = new acc::TextureGL(context, "character.png", 24, 24);

	my_shader->loadFile("vertex.vs", "fragment.fs");
	my_mesh->buildUnitQuad();

	acc::Game *game = (acc::Game *) new acc::Game(context);
	game->setBgColor(80, 80, 80, 0xff);
	//acc::Game *game = (acc::Game *) new MyGame(context);

	game->loadResource((acc::Resource *) new acc::Shader("my_shader", "shaders/sprite.vs", "shaders/sprite.fs"));
	game->loadResource((acc::Resource *) new acc::Shader("other_shader", "shaders/brush.vs", "shaders/brush.fs"));
	game->loadResource((acc::Resource *) new acc::Shader("cube_shader", "shaders/octree.vs", "shaders/octree.fs"));
	game->loadResource((acc::Resource *) texture);
	game->loadResource((acc::Resource *) new acc::TextureGL(context, "wall.png", 64, 64));
	game->loadResource((acc::Resource *) new acc::TextureGL(context, "floor.png", 64, 64));

	game->setScene((acc::Scene *) new acc::Scene3d(game));

	game->run();

	delete game;
}
