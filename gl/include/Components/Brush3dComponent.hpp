#ifndef BRUSH3D_COMPONENT_HPP
#define BRUSH3D_COMPONENT_HPP

#include "Model.hpp"
#include "TextureGL.hpp"

namespace acc {

struct Brush3dComponent {
	Mat4 model;

	Model3d *model3d;
	Shader *shader;

	Brush3dComponent(Model3d *model3d, Shader *shader);
	Brush3dComponent(Model3d *model3d, Resource *shader);
};

};

#endif
