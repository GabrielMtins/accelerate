#include "acc.hpp"
#include "RendererGL.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Mat4.hpp"
#include "TextureGL.hpp"

acc::TextureGL *texture;

class MyGame : public acc::Game {
	public:
		MyGame(acc::Context *context) : acc::Game(context){
		}

		void update(void){
			getContext()->clearScreen(0x00, 0x00, 0x00, 0xff);
			texture->renderCellEx(getContext(), 0, 0, -1, 5.0f, 5.0f, 0.0f, 0.0f, 0.0f, false, false);
			getContext()->renderPresent();
		}
};

int main(int argc, char **argv){
	acc::RendererGL::init();

	acc::Context *context = new acc::Context("test", 640, 480, SDL_WINDOW_OPENGL);
	context->setRenderer((acc::Renderer *) new acc::RendererGL(context));

	texture = new acc::TextureGL(context, "wall.png", 64, 64);

	acc::Game *game = (acc::Game *) new MyGame(context);

	game->run();

	delete game;
}
