#ifndef RENDER2D_SYSTEM_HPP
#define RENDER2D_SYSTEM_HPP

#include "SystemManager.hpp"
#include "Components/Components.hpp"
#include "Context.hpp"

#include <vector>

namespace acc {

enum RENDERDATATYPE {
	RENDER_DATA_SPRITE = 0,
	RENDER_DATA_RECT,
	RENDER_DATA_TILESET
};

struct RenderData {
	RenderData(SpriteComponent *sprite);
	RenderData(DrawRectComponent *rect);
	RenderData(TilesetComponent *tileset);

	union {
		SpriteComponent *sprite;
		DrawRectComponent *rect;
		TilesetComponent *tileset;
	} data;

	int type;
	int layer;
	int y;
};

class Render2dSystem : public System {
	public:
		Render2dSystem(Context *context, Vec3 *camera_position);
		void update(ComponentManager *component_manager);
	
	private:
		void updateText(ComponentManager *component_manager);
		void updateSprites(ComponentManager *component_manager);
		void updateDrawRects(ComponentManager *component_manager);
		void updateTileset(ComponentManager *component_manager);
		void renderTilesetComponent(TilesetComponent* tileset);
		void renderAll(void);
		bool isSpriteOnCamera(const SpriteComponent &sprite);
		bool isRectOnCamera(const DrawRectComponent &rect);
		static bool customTextureLess(const RenderData& a, const RenderData& b);

		Context *context;
		Vec3 *camera_position;

		std::vector<RenderData> render_array;
};

};

#endif
