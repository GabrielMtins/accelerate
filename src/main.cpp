#include <iostream>

#include "Scene2d.hpp"
#include "Game.hpp"
#include "ComponentArray.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"

int main(int argc, char **argv){
	acc::Game *game;
	
	game = new acc::Game("oi", 400, 240);

	game->setScene((acc::Scene *) new acc::Scene2d(game));

	game->run();

	delete game;

	return 0;
}
