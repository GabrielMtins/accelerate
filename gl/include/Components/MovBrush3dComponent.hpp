#ifndef MOV3DBRUSH_COMPONENT_HPP
#define MOV3DBRUSH_COMPONENT_HPP

#include "BrushBuilder.hpp"
#include "Shader.hpp"

namespace acc {

struct MovBrush3dComponent {
	Vec3 position;

	BrushBuilder *brush;
	Shader *shader;

	int collision_layer;

	MovBrush3dComponent(BrushBuilder *brush, Shader *shader);
	MovBrush3dComponent(BrushBuilder *brush, Resource *shader);

	void setCollisionLayer(int layer);
};

};

#endif
