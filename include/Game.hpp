#ifndef GAME_HPP
#define GAME_HPP

#include "Resource.hpp"
#include "Context.hpp"

namespace acc {

class Scene;

class Game {
	public:
		Game(const char *title, int internal_width, int internal_height);
		void setBgColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void setScene(Scene *scene);
		void loadResource(Resource *resource);
		Resource * getResource(std::string filename);
		void update(void);
		void loop(void);
		void run(void);
		Context * getContext(void);
		~Game(void);

	private:
		Scene *next_scene;
		Scene *current_scene;
		Context *context;
		ResourceManager *resource_manager;
		uint8_t bg_color[4];

};

};

#endif
