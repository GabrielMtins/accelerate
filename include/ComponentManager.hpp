#ifndef COMPONENT_MANAGER_HPP
#define COMPONENT_MANAGER_HPP

#include <typeindex>

#include "ComponentArray.hpp"

namespace acc{

class ComponentManager {
	public:
		template <typename T>
		void registerComponent(void){
			component_arrays[std::type_index(typeid(T))] = new ComponentArray<T>();
		}

		template <typename T>
		bool hasComponent(Entity entity){
			return getComponentArray<T>()->hasEntity(entity);
		}

		template <typename T>
		bool addComponent(Entity entity, T component){
			return getComponentArray<T>()->insertComponent(entity, component);
		}

		template <typename T>
		bool addComponent(Entity entity){
			return getComponentArray<T>()->insertComponent(entity, T());
		}

		template <typename T>
		bool removeComponent(Entity entity){
			return getComponentArray<T>()->removeComponent(entity);
		}

		template <typename T>
		T& getComponent(Entity entity){
			return getComponentArray<T>()->getComponent(entity);
		}
		
		template <typename T>
		ComponentArray<T> * getComponentArray(void){
			return (ComponentArray<T> *) component_arrays[std::type_index(typeid(T))];
		}

		template <typename T>
		bool hasComponentArray(void) {
			return component_arrays.find(std::type_index(typeid(T))) != component_arrays.end();
		}

		void removeEntity(Entity entity) {
			for(auto& it: component_arrays){
				if(it.second->hasEntity(entity)){
					it.second->removeComponent(entity);
				}
			}
		}

		~ComponentManager(void){
			for(auto& it: component_arrays){
				delete it.second;
			}
		}
			
	private:
		std::unordered_map<std::type_index, BaseComponentArray *> component_arrays;
};

};

#endif
