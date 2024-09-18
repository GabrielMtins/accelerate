#include "Game.hpp"
#include "Scene.hpp"

#include "Texture.hpp"

#include <iostream>

namespace acc {

Game::Game(const char *title, int internal_width, int internal_height){
	context = new Context(title, internal_width, internal_height);
	resource_manager = new ResourceManager();
	current_scene = NULL;

	loadResource((Resource *) new Texture(context, "player.png", 32, 32));
}

void Game::setScene(Scene *scene){
	this->current_scene = scene;
}

void Game::loadResource(Resource *resource){
	resource_manager->loadResource(resource);
}

Resource * Game::getResource(std::string filename){
	return resource_manager->getResource(filename);
}

void Game::update(void){
	context->clearScreen(100, 100, 100, 255);

	if(current_scene != NULL){
		current_scene->update();
	}

	context->renderPresent();
}

void Game::loop(void){
	context->pollEvent();

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

Game::~Game(void){
	if(current_scene != NULL)
		delete current_scene;
	delete context;
	delete resource_manager;
}

};