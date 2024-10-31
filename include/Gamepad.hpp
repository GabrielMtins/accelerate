#ifndef GAMEPAD_HPP
#define GAMEPAD_HPP

#include "Context.hpp"

namespace acc {

/* Class for using Xinput gamepads */
class Gamepad {
	public:
		Gamepad(Context *context, int index);
		Gamepad(Context *context);

		bool open(int index);
		void update(void);

		int16_t getAxis(const std::string& key);
		bool getButton(const std::string& key);
		bool getButtonDown(const std::string& key);
		bool getButtonUp(const std::string& key);

		~Gamepad(void);

		static int numGamepads(void);

	private:
		void setup(void);

		SDL_GameController *gamepad;
		Context *context;

		bool button_state[SDL_CONTROLLER_BUTTON_MAX];

		uint64_t button_tick_pressed[SDL_CONTROLLER_BUTTON_MAX];
		uint64_t button_tick_released[SDL_CONTROLLER_BUTTON_MAX];

		static std::unordered_map<std::string, int> string_to_button;
		static std::unordered_map<std::string, int> string_to_axis;
};

};

#endif
