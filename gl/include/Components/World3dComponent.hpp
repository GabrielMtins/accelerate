#ifndef WORLD_3D_COMPONENT_HPP
#define WORLD_3D_COMPONENT_HPP

#include "BrushBuilder.hpp"
#include "Shader.hpp"

namespace acc {

struct World3dComponent {
	std::vector<BrushBuilder *> brushes;
	Shader *shader;

	uint32_t collision_layer;

	World3dComponent(void);
	World3dComponent(Shader *shader);
	World3dComponent(Resource *shader);
	void setCollisionLayer(int layer);
};
	
};

#endif
