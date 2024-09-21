#include "Scene.hpp"
#include "Game.hpp"

namespace acc {

Scene::Scene(Game *game){
	this->game = game;

	entity_manager = new EntityManager();
	component_manager = new ComponentManager();
	system_manager = new SystemManager();
}

Game * Scene::getGame(void){
	return game;
}

ComponentManager * Scene::getComponentManager(void){
	return component_manager;
}

void Scene::update(void){
	system_manager->update(component_manager);

	while(!destroyed_entities.empty()){
		Entity entity = destroyed_entities.front();
		removeEntity(entity);
		destroyed_entities.pop();
	}
}

void Scene::addToDestroyQueue(Entity entity){
	destroyed_entities.push(entity);
}

Scene::~Scene(){
	delete entity_manager;
	delete component_manager;
	delete system_manager;
}

Entity Scene::getNextEntity(void){
	return entity_manager->getNextEntity();
}

void Scene::removeEntity(Entity entity){
	component_manager->removeEntity(entity);
	entity_manager->removeEntity(entity);
}

};
