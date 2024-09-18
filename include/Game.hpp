#ifndef GAME_HPP
#define GAME_HPP

#include "Resource.hpp"
#include "Context.hpp"

namespace acc {

class Scene;

class Game {
	public:
		Game(const char *title, int internal_width, int internal_height);
		void setScene(Scene *scene);
		void loadResource(Resource *resource);
		Resource * getResource(std::string filename);
		void update(void);
		void loop(void);
		void run(void);
		Context * getContext(void);
		~Game(void);

	private:
		Scene *current_scene;
		Context *context;
		ResourceManager *resource_manager;

};

};

#endif
