#ifndef RENDERPORTAL3D_HPP
#define RENDERPORTAL3D_HPP

#include "Canvas.hpp"
#include "Texture.hpp"
#include "SystemManager.hpp"
#include "ScenePortal3d.hpp"
#include "Components/PortalWorldComponent.hpp"

#include <unordered_map>

#define INV_Z_SIZE 4096

#define DIST_MAX_Z 20.0f

namespace acc {

enum WallType {
	WALL_NORMAL = 0,
	WALL_PORTAL_BOTTOM,
	WALL_PORTAL_TOP,
};

class RenderPortal3d : public System {
	public:
		RenderPortal3d(ScenePortal3d *scene);
		void update(ComponentManager *component_manager);
		~RenderPortal3d(void);
	
	private:
		void precalc(void);
		void clearValuesY(void);
		void clearDepthBuffer(void);
		void updateCameraPosition(PortalWorldComponent& world, Vec3 new_position);
		void render(PortalWorldComponent &world);
		void renderSector(PortalWorldComponent& world, int sector_id, int min_x, int max_x, std::unordered_set<int> sectors_rendered);
		void renderWall(PortalWorldComponent& world, Sector& sector, Wall& wall, Vec3& start, int min_x, int max_x, int type);
		void renderPortalFrame(PortalWorldComponent& world, Sector& sector, Wall& wall, Vec3& start, int min_x, int max_x);
		void renderPlane(TextureConfiguration& config, float height, float x, float start_y, float end_y);

		bool isWallOnCameraDirection(Wall &wall);
		float minDistanceFromCamera(Wall &wall);
		float maxDistanceFromCamera(Wall &wall);
		bool isWallVisible(Wall &wall, int min_x, int max_x);
		Wall clipWall(Wall &wall);

		float getProjectionDistance(Vec3 point);
		float getProjectionOrtho(Vec3 point);
		float getProjectionX(float projection_x, float distance);
		float getProjectionY(float height, float distance);

		Vec3 getVec3Inverse(float height, float i, float j);
		
		void putPixel(Color color, int i, int j);

		void setDepthBuffer(float depth, int i, int j);
		float getDepthBuffer(int i, int j);

		Color getTileColor(int id, Vec3 uv);

		Vec3 camera_position;
		Vec3 camera_direction;
		float camera_angle;
		int camera_sector;

		ScenePortal3d *scene;
		Canvas *framebuffer_canvas;
		Canvas *tilemap_canvas;
		Texture *framebuffer_texture;

		int tile_width, tile_height;

		int framebuffer_width, framebuffer_height;

		float *top_value_y;
		float *bottom_value_y;

		float *div_table_x;
		float *div_table_y;

		float *depth_buffer;
};

};

#endif
