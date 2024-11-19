#ifndef RENDER3D_HPP
#define RENDER3D_HPP

#include "SystemManager.hpp"
#include "Context.hpp"
#include "Mesh.hpp"

namespace acc {

class Render3dSystem : public System {
	public:
		Render3dSystem(Context *context, Mat4 *view);
		void update(ComponentManager *component_manager);

	private:
		void updateSprites(ComponentManager *component_manager);
		void updateBrushes(ComponentManager *component_manager);

		Context *context;
		Mat4 *view;
		Mat4 projection;

		Mesh *sprite_mesh;
};

};

#endif
