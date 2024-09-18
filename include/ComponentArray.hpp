#ifndef COMPONENT_ARRAY_HPP
#define COMPONENT_ARRAY_HPP

#include <unordered_map>
#include <array>

#include "global.hpp"

namespace acc{

class BaseComponentArray {
	public:
		virtual ~BaseComponentArray(void) = default;
		virtual bool removeComponent(Entity entity) = 0;
		virtual bool hasEntity(Entity entity) = 0;
};

template <typename T> class ComponentArray : public BaseComponentArray {
	public:
		ComponentArray(void){
			size = 0;
		}

		bool insertComponent(Entity entity, T component){
			if(entity_to_index.find(entity) != entity_to_index.end()){
				return false;
			}

			size_t new_index = size;

			component_array[new_index] = component;
			entity_to_index[entity] = new_index;
			index_to_entity[new_index] = entity;

			size++;

			return true;
		}

		bool removeComponent(Entity entity) override{
			if(entity_to_index.find(entity) == entity_to_index.end()){
				return false;
			}
			if(size == 0)
				return false;

			if(size == 1){
				size--;
				return true;
			}

			/* pick up the last component and swap places */
			size_t old_index = entity_to_index[entity];
			
			size_t last_index = size - 1;
			Entity last_entity = index_to_entity[last_index];

			component_array[old_index] = component_array[last_index];
			entity_to_index[last_entity] = old_index;
			index_to_entity[old_index] = last_entity;

			entity_to_index.erase(entity);
			index_to_entity.erase(last_index);

			size--;

			return true;
		}

		T& getComponent(Entity entity){
			return component_array[entity_to_index[entity]];
		}

		T& atIndex(size_t index){
			return component_array[index];
		}

		std::array<T, MAX_ENTITIES>& getComponentArray(void){
			return component_array;
		}

		size_t entityToIndex(Entity entity){
			return entity_to_index[entity];
		}

		Entity indexToEntity(size_t index){
			return index_to_entity[index];
		}

		bool hasEntity(Entity entity){
			return entity_to_index.find(entity) != entity_to_index.end();
		}

		size_t getSize(void){
			return size;
		}

	private:
		std::array<T, MAX_ENTITIES> component_array;
		std::unordered_map<Entity, size_t> entity_to_index;
		std::unordered_map<size_t, Entity> index_to_entity;

		size_t size;
};

};

#endif
