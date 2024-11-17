#include "Context.hpp"
#include "Game.hpp"
#include "Scene.hpp"

#include "Texture.hpp"
#include "Font.hpp"

#include <cmath>

namespace acc {

Game::Game(Context *context){
	this->context = context;
	resource_manager = new ResourceManager();
	current_scene = NULL;
	next_scene = NULL;
	swap_scene = NULL;

	bg_color[0] = bg_color[1] = bg_color[2] = 100;
	bg_color[3] = 0xff;
}

void Game::setBgColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	bg_color[0] = r;
	bg_color[1] = g;
	bg_color[2] = b;
	bg_color[3] = a;
}

void Game::setScene(Scene *scene){
	this->next_scene = scene;
}

Scene * Game::swapScene(Scene *new_scene){
	this->swap_scene = new_scene;
	return current_scene;
}

void Game::loadResource(Resource *resource){
	resource_manager->loadResource(resource);
}

Resource * Game::getResource(const std::string& filename){
	return resource_manager->getResource(filename);
}

void Game::update(void){
	context->clearScreen(bg_color[0], bg_color[1], bg_color[2], bg_color[3]);

	if(current_scene != NULL){
		current_scene->update();
	}

	context->renderPresent();
}

void Game::loop(void){
	context->pollEvent();

	for(auto i : gamepads){
		i->update();
	}

	if(next_scene != NULL){
		if(current_scene != NULL)
			delete current_scene;

		current_scene = next_scene;
		next_scene = NULL;
	}

	if(swap_scene != NULL){
		current_scene = swap_scene;
		swap_scene = NULL;
	}

	update();

	context->updateTime();
}

void Game::run(void){
	while(context->isRunning()){
		loop();
	}
}

Context * Game::getContext(void){
	return context;
}

void Game::addGamepad(Gamepad *gamepad){
	gamepads.push_back(gamepad);
}

Game::~Game(void){
	if(current_scene != NULL)
		delete current_scene;

	if(next_scene != NULL)
		delete next_scene;

	delete resource_manager;
	delete context;
}

};
