#ifndef BEHAVIOR_SYSTEM_HPP
#define BEHAVIOR_SYSTEM_HPP

#include "SystemManager.hpp"
#include "Scene.hpp"

namespace acc {

class BehaviorSystem : public System {
	public:
		BehaviorSystem(Scene *scene);
		void update(ComponentManager *component_manager);
	
	private:
		Scene *scene;
};

};

#endif
