#ifndef SYSTEM_MANAGER_HPP
#define SYSTEM_MANAGER_HPP

#include <vector>
#include "ComponentArray.hpp"
#include "ComponentManager.hpp"

namespace acc{

class System {
	public:
		virtual ~System(void) = default;
		virtual void update(ComponentManager *component_manager);
};

class SystemManager {
	public:
		bool addSystem(System *system);
		bool update(ComponentManager *component_manager);
		~SystemManager(void);

	private:
		std::vector<System *> systems;
};

};

#endif
