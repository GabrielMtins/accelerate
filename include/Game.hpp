#ifndef GAME_HPP
#define GAME_HPP

#include "Resource.hpp"
#include "Context.hpp"
#include "Gamepad.hpp"

#include <vector>

namespace acc {

class Scene;

class Game {
	public:
		/**
		 * The same constructor as the Context class.
		 */
		Game(Context *context);
		/**
		 * Set the background color of the scene.
		 */
		void setBgColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		/**
		 * Sets the next scene to be loaded. It'll be loaded on the next frame.
		 */
		void setScene(Scene *scene);
		/**
		 * Sets a new scene and returns the old one.
		 */
		Scene * swapScene(Scene *new_scene);
		/**
		 * Loads a resource into the resource manager of the game.
		 */
		void loadResource(Resource *resource);
		/**
		 * Returns the resource with the name given on the first argument.
		 * If the resource doesn't exist, it returns NULL.
		 */
		Resource * getResource(const std::string& filename);
		virtual void update(void);
		void loop(void);
		void run(void);
		Context * getContext(void);
		void addGamepad(Gamepad *gamepad);
		virtual ~Game(void);

	private:
		Scene *next_scene;
		Scene *current_scene;
		Scene *swap_scene;

		Context *context;
		ResourceManager *resource_manager;
		uint8_t bg_color[4];

		std::vector<Gamepad *> gamepads;

};

};

#endif
