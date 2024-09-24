#ifndef SCENE2D_HPP
#define SCENE2D_HPP

#include "Scene.hpp"
#include "Vec3.hpp"

namespace acc {

class Scene2d : public Scene {
	public:
		Scene2d(Game *game);
		void setCameraPosition(Vec3 camera_position);
		Vec3 getCameraPosition(void);

	private:
		Vec3 camera_position;
};

};

#endif
