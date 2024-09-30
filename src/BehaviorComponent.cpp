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
	game = scene->getGame();
	context = game->getContext();
	component_manager = scene->getComponentManager();
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
	return context;
}

bool BehaviorFunction::getKeyUp(std::string key){
	return context->getKeyUp(key);
}

bool BehaviorFunction::getKeyDown(std::string key){
	return context->getKeyDown(key);
}

bool BehaviorFunction::getKey(std::string key){
	return context->getKey(key);
}

bool BehaviorFunction::getMouseButtonUp(std::string key){
	return context->getMouseButtonUp(key);
}

bool BehaviorFunction::getMouseButtonDown(std::string key){
	return context->getMouseButtonDown(key);
}

bool BehaviorFunction::getMouseButton(std::string key){
	return context->getMouseButton(key);
}

bool BehaviorFunction::isEntityManaged(Entity entity){
	return scene->isEntityManaged(entity);
}

bool BehaviorFunction::raycast(Vec3 origin, Vec3 direction, uint32_t layer_mask, Entity *return_entity, Vec3 *return_intersection){
	return PhysicsSystem::raycast(
			component_manager,
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

	if(behavior.behavior_function->scene == NULL)
		return false;

	behavior.behavior_function->onMessage(this->entity, message);

	return true;
}

Vec3 BehaviorFunction::getMousePosition(void){
	int x, y;

	context->getMousePosition(&x, &y);

	return Vec3(x, y);
}

float BehaviorFunction::getDeltaTime(void){
	return context->getDeltaTime();
}

uint64_t BehaviorFunction::getTicks(void){
	return context->getTicks();
}

void BehaviorFunction::destroy(Entity entity){
	scene->addToDestroyQueue(entity);
}

void BehaviorFunction::destroy(void){
	scene->addToDestroyQueue(entity);
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
