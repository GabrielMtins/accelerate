#ifndef BRUSH3D_COMPONENT_HPP
#define BRUSH3D_COMPONENT_HPP

#include "Model3d.hpp"
#include "TextureGL.hpp"

namespace acc {

struct Object3dComponent {
	Mat4 model;

	Model3d *model3d;
	Shader *shader;

	Object3dComponent(Model3d *model3d, Shader *shader);
	Object3dComponent(Model3d *model3d, Resource *shader);
};

};

#endif
