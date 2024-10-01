#ifndef SCENEPORTAL3D_HPP
#define SCENEPORTAL3D_HPP

#include "Scene2d.hpp"
#include "Texture.hpp"
#include "Canvas.hpp"

namespace acc {

class ScenePortal3d : public Scene2d {
	public:
		ScenePortal3d(Game *game);
		Canvas * getFramebufferCanvas(void);
		Texture * getFramebufferTexture(void);
		virtual ~ScenePortal3d(void);

	private:
		Canvas *framebuffer_canvas;
		Texture *framebuffer_texture;
};

};

#endif
