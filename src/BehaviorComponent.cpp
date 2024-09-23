#include "Components/BehaviorComponent.hpp"

namespace acc {

BehaviorFunction::BehaviorFunction(void){
	scene = NULL;
	has_created = false;
}

void BehaviorFunction::onCreate(void){
}

void BehaviorFunction::onUpdate(void){
}

void BehaviorFunction::onCollision(Entity other){
	(void) other;
}

bool BehaviorFunction::hasCreated(void){
	return has_created;
}

void BehaviorFunction::setAsCreated(void){
	has_created = true;
}

void BehaviorFunction::setScene(Scene *scene){
	this->scene = scene;
}

void BehaviorFunction::setEntity(Entity entity){
	this->entity = entity;
}

Scene * BehaviorFunction::getScene(void){
	return scene;
}

Game * BehaviorFunction::getGame(void){
	return scene->getGame();
}

Context * BehaviorFunction::getContext(void){
	return scene->getGame()->getContext();
}

bool BehaviorFunction::getKeyUp(std::string key){
	return getContext()->getKeyUp(key);
}

bool BehaviorFunction::getKeyDown(std::string key){
	return getContext()->getKeyDown(key);
}

bool BehaviorFunction::getKey(std::string key){
	return getContext()->getKey(key);
}

bool BehaviorFunction::getMouseButtonUp(std::string key){
	return getContext()->getMouseButtonUp(key);
}

bool BehaviorFunction::getMouseButtonDown(std::string key){
	return getContext()->getMouseButtonDown(key);
}

bool BehaviorFunction::getMouseButton(std::string key){
	return getContext()->getMouseButton(key);
}

Vec3 BehaviorFunction::getMousePosition(void){
	int x, y;

	getContext()->getMousePosition(&x, &y);

	return Vec3(x, y);
}

float BehaviorFunction::getDeltaTime(void){
	return getContext()->getDeltaTime();
}

uint64_t BehaviorFunction::getTicks(void){
	return getContext()->getTicks();
}

void BehaviorFunction::destroy(Entity entity){
	getScene()->addToDestroyQueue(entity);
}

void BehaviorFunction::destroy(void){
	getScene()->addToDestroyQueue(entity);
}

BehaviorComponent::BehaviorComponent(){
	this->behavior_function = nullptr;
}

void BehaviorComponent::setFunction(std::shared_ptr<BehaviorFunction> behavior_function){
	this->behavior_function = behavior_function;
}

BehaviorComponent::~BehaviorComponent(void){
}

};
