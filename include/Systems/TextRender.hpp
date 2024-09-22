#ifndef TEXT_RENDER_HPP
#define TEXT_RENDER_HPP

#include "SystemManager.hpp"
#include "Context.hpp"
#include "Scene.hpp"

namespace acc {

class TextRender : public System {
	public:
		TextRender(Scene *scene);
		void update(ComponentManager *component_manager);
	
	private:
		Context *context;
};

};

#endif
