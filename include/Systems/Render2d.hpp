#ifndef RENDER2D_SYSTEM_HPP
#define RENDER2D_SYSTEM_HPP

#include "SystemManager.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"
#include "Context.hpp"

#include <vector>

namespace acc {

class Render2dSystem : public System {
	public:
		Render2dSystem(Context *context, Vec3 *camera_position);
		void update(ComponentManager *component_manager);
	
	private:
		Context *context;
		Vec3 *camera_position;
		static bool customTextureLess(SpriteComponent *a, SpriteComponent *b);
};

};

#endif
