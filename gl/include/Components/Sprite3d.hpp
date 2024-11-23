#ifndef SPRITE_3D_COMPONENT_HPP
#define SPRITE_3D_COMPONENT_HPP

#include "Shader.hpp"
#include "Mesh.hpp"
#include "Vec3.hpp"
#include "TextureGL.hpp"
#include "Mat4.hpp"

namespace acc {

struct Sprite3dComponent {
	Vec3 position;
	Mat4 scale;
	Mat4 rotation;

	Mat4 model;

	Shader *shader;
	TextureGL *texture;

	bool flip_horizontally;
	bool flip_vertically;

	int id;

	Sprite3dComponent(Shader *shader, TextureGL *texture);
	Sprite3dComponent(Resource *shader, Resource *texture);
};

};

#endif
