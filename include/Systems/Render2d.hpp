#ifndef RENDER2D_SYSTEM_HPP
#define RENDER2D_SYSTEM_HPP

#include "SystemManager.hpp"
#include "Components/Components.hpp"
#include "Context.hpp"

#include <vector>

namespace acc {

class Render2dSystem : public System {
	public:
		Render2dSystem(Context *context, Vec3 *camera_position);
		void update(ComponentManager *component_manager);
	
	private:
		void updateText(ComponentManager *component_manager);
		void updateSprites(ComponentManager *component_manager);
		void updateTileset(ComponentManager *component_manager);
		void renderTilesetComponent(TilesetComponent& tileset);
		bool isSpriteOnCamera(const SpriteComponent &sprite);
		static bool customTextureLess(SpriteComponent *a, SpriteComponent *b);

		Context *context;
		Vec3 *camera_position;
};

};

#endif
