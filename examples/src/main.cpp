#include "Game.hpp"
#include "Scene2d.hpp"
#include "TestScene.hpp"
#include "Texture.hpp"
#include "Font.hpp"

int main(int argc, char **argv){
	acc::Game *game;

	game = new acc::Game("oi", 400, 240);

	game->loadResource((acc::Resource *) new acc::Texture(game->getContext(), "test.png", 32, 32));
	game->loadResource((acc::Resource *) new acc::Texture(game->getContext(), "player.png", 32, 32));
	game->loadResource((acc::Resource *) new acc::Texture(game->getContext(), "texture.png", 16, 16));
	game->loadResource((acc::Resource *) new acc::Font("default.ttf", 12));

	game->setScene((acc::Scene *) new TestScene(game));

	game->run();

	delete game;

	return 0;
}
