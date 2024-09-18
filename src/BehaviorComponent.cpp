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

BehaviorComponent::BehaviorComponent(){
	this->behavior_function = NULL;
}

void BehaviorComponent::setFunction(BehaviorFunction *behavior_function){
	this->behavior_function = behavior_function;
}

BehaviorComponent::~BehaviorComponent(void){
	if(this->behavior_function != NULL){
		delete this->behavior_function;
	}
}

};