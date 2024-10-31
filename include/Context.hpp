#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <unordered_map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

namespace acc {

class Context {
	public:
		/**
		 * The constructor of a context, the context of the application which
		 * contains information like window structs, renderers etc.
		 * The first argument is a string, for the title of the game.
		 * The second argument is the internal width of the window.
		 * The third argument is the internal height of the window.
		 * The internal width and height are the width and height used by the
		 * renderer. If you want to create a pixel art game, you can use
		 * a internal width of (320x240) and can use that system coordinates.
		 * It'll scale accordly.
		 */
		Context(const char *title, int internal_width, int internal_height);
		void close(void);
		void setMinimumFps(uint32_t fps);
		void setFps(uint32_t fps);
		void setWindowSize(int width, int height);
		void maximizeWindow(void);
		void setFullscreenMode(void);
		void setWindowedMode(void);
		void showCursor(bool enable);
		int getWidth(void);
		int getHeight(void);
		bool isRunning(void);
		float getDeltaTime(void);
		void pollEvent(void);
		void updateTime(void);
		uint64_t getTicks(void);
		void clearScreen(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void renderPresent(void);
		bool getKey(const std::string& key);
		bool getKeyDown(const std::string& key);
		bool getKeyUp(const std::string& key);
		std::unordered_map<std::string, int>& getStringToKeys(void);
		std::string getTextInput(void);
		void getMousePosition(int *x, int *y);
		bool getMouseButton(const std::string& mouse_button);
		bool getMouseButtonDown(const std::string& mouse_button);
		bool getMouseButtonUp(const std::string& mouse_button);
		SDL_Renderer * getRenderer(void);
		~Context(void);

	private:
		void setUpKeys(void);

		SDL_Window *window;
		SDL_Renderer *renderer;

		bool key_state[SDL_NUM_SCANCODES];
		uint64_t key_tick_pressed[SDL_NUM_SCANCODES];
		uint64_t key_tick_released[SDL_NUM_SCANCODES];

		std::unordered_map<std::string, bool> mouse_button_state;
		std::unordered_map<std::string, uint64_t> mouse_button_tick_pressed;
		std::unordered_map<std::string, uint64_t> mouse_button_tick_released;

		std::string text_input;

		std::unordered_map<std::string, int> string_to_keys;

		float delta_time;
		uint64_t first_time;
		uint32_t fps;
		float minimum_delta;
		bool quit;

		int window_width, window_height;

		int internal_width, internal_height;
};

};

#endif
