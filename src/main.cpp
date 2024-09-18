#include <iostream>

#include "Scene2d.hpp"
#include "Game.hpp"
#include "ComponentArray.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"

namespace acc {
	class IntSystem : public System {
		public:
			void update(ComponentManager *component_manager) override{
				auto arr = component_manager->getComponentArray<int>();

				for(size_t i = 0; i < arr->getSize(); i++){
					std::cout << arr->atIndex(i)++ << '\n';
				}
			}
	};
}

int main(void){
	acc::Game *game;
	
	game = new acc::Game("oi", 400, 240);

	game->setScene((acc::Scene *) new acc::Scene2d(game));

	game->run();

	delete game;

	return 0;
}
