#ifndef BRUSH_BUILDER_HPP
#define BRUSH_BUILDER_HPP

#include "Model.hpp"

namespace acc {

class BrushBuilder {
	public:
		BrushBuilder(void);

		Model3d * build(void);

		void setGeometry(const std::vector<Vec3>& vertices);
		void setBottomProperties(TextureGL *bottom_texture, float bottom_height, Vec3 bottom_offset, Vec3 bottom_scaling);
		void setTopProperties(TextureGL *top_texture, float top_height, Vec3 top_offset, Vec3 top_scaling);
		void setWallProperties(TextureGL *wall_texture, Vec3 wall_offset, Vec3 wall_scaling);

	private:
		Mesh * buildFlat(float height, const Vec3& offset, const Vec3& scale);
		Mesh * buildWall(void);

		std::vector<Vec3> vertices;
		TextureGL *bottom_texture, *top_texture, *wall_texture;
		float bottom_height, top_height;

		Vec3 wall_offset, wall_scaling, bottom_offset, bottom_scaling, top_offset, top_scaling;
};

};

#endif
