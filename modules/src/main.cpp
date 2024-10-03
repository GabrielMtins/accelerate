#include "Game.hpp"
#include "ScenePortal3d.hpp"
#include "Canvas.hpp"

using namespace acc;

int main(int argc, char **argv){
	(void) (argc);
	(void) (argv);

	Game *game = new Game("portals", 426, 240);

	game->loadResource((Resource *) new Canvas("tile_canvas.png"));

	game->setScene((Scene *) new ScenePortal3d(game));

	game->run();

	delete game;

	return 0;
}
