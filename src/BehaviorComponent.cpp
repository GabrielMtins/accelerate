#include "Components/BehaviorComponent.hpp"

#include "Systems/Physics.hpp"

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

void BehaviorFunction::onMessage(Entity sender, std::string message){
	(void) sender;
	(void) message;
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

bool BehaviorFunction::raycast(Vec3 origin, Vec3 direction, uint32_t layer_mask, Entity *return_entity, Vec3 *return_intersection){
	return PhysicsSystem::raycast(
			getScene()->getComponentManager(),
			origin,
			direction,
			layer_mask,
			return_entity,
			return_intersection
			);
}

bool BehaviorFunction::sendMessage(Entity receiver, std::string message){
	if(!hasComponent<BehaviorComponent>(receiver))
		return false;

	auto& behavior = getComponent<BehaviorComponent>(receiver);

	if(behavior.behavior_function == nullptr)
		return false;

	if(behavior.behavior_function->getScene() == NULL)
		return false;

	behavior.behavior_function->onMessage(this->entity, message);

	return true;
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
