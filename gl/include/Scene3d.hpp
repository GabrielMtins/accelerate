#ifndef SCENE3D_HPP
#define SCENE3D_HPP

#include "Scene2d.hpp"
#include "Mat4.hpp"

namespace acc {

class Scene3d : public Scene2d {
	public:
		Scene3d(Game *game);
		void setViewMatrix(const Mat4& view);
	
	private:
		Mat4 view;
};

};

#endif
