#include "Resource.hpp"

namespace acc {

ResourceManager::ResourceManager(void){
}

void ResourceManager::loadResource(Resource *resource){
	resources[resource->getName()] = resource;
}

Resource * ResourceManager::getResource(std::string filename){
	if(resources.find(filename) == resources.end()) return NULL;

	return resources[filename];
}

ResourceManager::~ResourceManager(void){
	for(auto i : resources){
		delete i.second;
	}
}

};
