#ifndef BRUSH_BUILDER_HPP
#define BRUSH_BUILDER_HPP

#include "Model.hpp"

namespace acc {

class BrushBuilder {
	public:
		BrushBuilder(void);

		Model3d * build(void);
		void reset(void);
		Model3d * getModel(void);

		void setGeometry(const std::vector<Vec3>& vertices);
		void setBottomProperties(TextureGL *bottom_texture, float bottom_height, Vec3 bottom_offset, Vec3 bottom_scaling);
		void setTopProperties(TextureGL *top_texture, float top_height, Vec3 top_offset, Vec3 top_scaling);
		void setWallProperties(TextureGL *wall_texture, Vec3 wall_offset, Vec3 wall_scaling);
		void setTopRamp(float inclination, int wall_index);
		void setBottomRamp(float inclination, int wall_index);

		bool checkCollisionCylinder(Vec3 position, float radius, float height, float tolerance, Vec3 *delta);
		void getBoundingBox(Vec3 *start, Vec3 *size);

		std::vector<Vec3> vertices;
		std::vector<Vec3> normals;

		TextureGL *bottom_texture, *top_texture, *wall_texture;
		float bottom_height, top_height;
		float bottom_inclination, top_inclination;
		int bottom_ramp_wall_index, top_ramp_wall_index;

		Vec3 wall_offset, wall_scaling, bottom_offset, bottom_scaling, top_offset, top_scaling;

	private:
		Vec3 normal2D(Vec3 t);
		void computeNormals(void);
		bool checkCollisionPoint(Vec3 point);
		bool checkCollisionCircle(Vec3 position, float radius, Vec3 *delta);
		bool checkCollisionLine(Vec3 start, Vec3 end, Vec3 *intersection);
		float getHeight(int ramp_wall_index, float height, float inclination, const Vec3& position);
		Mesh * buildFlat(int ramp_wall_index, float inclination, float height, const Vec3& offset, const Vec3& scale);
		Mesh * buildWall(void);
		bool checkBoundingCylinder(Vec3 position, float radius, float height);
		void buildBoundingCylinder(void);

		Vec3 center;
		Vec3 aabb_start, aabb_size;
		float radius, min_height, max_height;

		Model3d *model3d;
};

};

#endif
