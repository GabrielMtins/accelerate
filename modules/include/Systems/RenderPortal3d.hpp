#ifndef RENDERPORTAL3D_HPP
#define RENDERPORTAL3D_HPP

#include "Canvas.hpp"
#include "Texture.hpp"
#include "SystemManager.hpp"
#include "ScenePortal3d.hpp"
#include "Components/PortalWorldComponent.hpp"

namespace acc {

class RenderPortal3d : public System {
	public:
		RenderPortal3d(ScenePortal3d *scene);
		void update(ComponentManager *component_manager);
	
	private:
		void render(PortalWorldComponent &world);
		void renderSector(Sector& sector);
		void renderWall(Sector& sector, Wall& wall, Vec3& start);

		bool isWallVisible(Wall &wall);
		Wall clipWall(Wall &wall);

		float getProjectionDistance(Vec3 point);
		float getProjectionOrtho(Vec3 point);
		float getProjectionX(float height, float distance);
		float getProjectionY(float height, float distance);
		
		void putPixel(Color color, int i, int j);

		Color getTileColor(int id, Vec3 uv);

		Vec3 camera_position;
		Vec3 camera_direction;
		float camera_angle;

		ScenePortal3d *scene;
		Canvas *framebuffer_canvas;
		Canvas *tilemap_canvas;
		Texture *framebuffer_texture;

		int tile_width, tile_height;
};

};

#endif
