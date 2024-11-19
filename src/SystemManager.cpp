#include "SystemManager.hpp"

#include <iostream>

namespace acc {

void System::update(ComponentManager *component_manager){
	(void) component_manager;
}

bool SystemManager::addSystemFront(System *system){
	systems.insert(systems.begin(), system);
	return true;
}

bool SystemManager::addSystem(System *system){
	systems.push_back(system);
	return true;
}

bool SystemManager::update(ComponentManager *component_manager){
	for(auto &i : systems){
		i->update(component_manager);
	}
	
	return true;
}

SystemManager::~SystemManager(void){
	for(auto &i : systems){
		delete i;
	}
}

};
