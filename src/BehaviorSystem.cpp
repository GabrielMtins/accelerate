#include "Systems/BehaviorSystem.hpp"
#include "Components/BehaviorComponent.hpp"

namespace acc {

BehaviorSystem::BehaviorSystem(Scene *scene){
	this->scene = scene;
}

void BehaviorSystem::update(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<BehaviorComponent>())
		return;

	auto arr = component_manager->getComponentArray<BehaviorComponent>();

	for(size_t i = 0; i < arr->getSize(); i++){
		Entity entity = arr->indexToEntity(i);

		auto& behavior_component = arr->atIndex(i);
		auto behavior_function = behavior_component.behavior_function;

		if(behavior_function == nullptr || behavior_function == NULL) return;

		behavior_function->setScene(scene);
		behavior_function->setEntity(entity);

		if(behavior_function->hasCreated()){
			behavior_function->onUpdate();
		}
		else{
			behavior_function->onCreate();
			behavior_function->setAsCreated();
		}
	}
}

};
