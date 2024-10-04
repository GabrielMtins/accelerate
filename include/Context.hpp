#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <unordered_map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define FPS_MAX 1000

namespace acc {

class Context {
	public:
		Context(const char *title, int internal_width, int internal_height);
		void maximizeWindow(void);
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
		bool getKey(std::string key);
		bool getKeyDown(std::string key);
		bool getKeyUp(std::string key);
		std::string getTextInput(void);
		void getMousePosition(int *x, int *y);
		bool getMouseButton(std::string mouse_button);
		bool getMouseButtonDown(std::string mouse_button);
		bool getMouseButtonUp(std::string mouse_button);
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
		bool quit;

		int window_width, window_height;

		int internal_width, internal_height;
};

};

#endif
