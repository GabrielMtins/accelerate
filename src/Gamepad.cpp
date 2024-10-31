#include "Gamepad.hpp"

namespace acc {

Gamepad::Gamepad(Context *context, int index){
	this->context = context;

	setup();
	open(index);
}

Gamepad::Gamepad(Context *context){
	this->context = context;

	setup();
}

bool Gamepad::open(int index){
	if(gamepad != NULL)
		SDL_GameControllerClose(gamepad);

	gamepad = SDL_GameControllerOpen(index);

	if(gamepad == NULL)
		return false;

	return true;
}

void Gamepad::update(void){
	for(int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++){
		if(SDL_GameControllerGetButton(gamepad, (SDL_GameControllerButton) i)){
			if(button_state[i] == false){
				button_tick_pressed[i] = context->getTicks();
			}

			button_state[i] = true;
		}
		else{
			if(button_state[i] == true){
				button_tick_released[i] = context->getTicks();
			}

			button_state[i] = false;
		}
	}
}

int16_t Gamepad::getAxis(const std::string& key){
	if(gamepad == NULL)
		return 0;

	if(string_to_axis.find(key) == string_to_axis.end())
		return 0;

	return SDL_GameControllerGetAxis(gamepad, (SDL_GameControllerAxis) string_to_axis[key]);
}

bool Gamepad::getButton(const std::string& key){
	if(string_to_button.find(key) == string_to_button.end())
		return false;

	int index = string_to_button[key];

	return button_state[index];
}

bool Gamepad::getButtonDown(const std::string& key){
	if(string_to_button.find(key) == string_to_button.end())
		return false;

	int index = string_to_button[key];

	return button_state[index] && (button_tick_pressed[index] == context->getTicks());
}

bool Gamepad::getButtonUp(const std::string& key){
	if(string_to_button.find(key) == string_to_button.end())
		return false;

	int index = string_to_button[key];

	return button_state[index] && (button_tick_released[index] == context->getTicks());
}

Gamepad::~Gamepad(void){
	if(gamepad != NULL)
		SDL_GameControllerClose(gamepad);
}

int Gamepad::numGamepads(void){
	return SDL_NumJoysticks();
}

void Gamepad::setup(void){
	gamepad = NULL;

	for(size_t i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++){
		button_tick_pressed[i] = 0;
		button_tick_released[i] = 0;
		button_state[i] = false;
	}
}

std::unordered_map<std::string, int> Gamepad::string_to_button = {
	{"a", SDL_CONTROLLER_BUTTON_A},
	{"b", SDL_CONTROLLER_BUTTON_A},
	{"x", SDL_CONTROLLER_BUTTON_X},
	{"y", SDL_CONTROLLER_BUTTON_Y},
	{"back", SDL_CONTROLLER_BUTTON_BACK},
	{"guide", SDL_CONTROLLER_BUTTON_GUIDE},
	{"start", SDL_CONTROLLER_BUTTON_START},
	{"lstick", SDL_CONTROLLER_BUTTON_LEFTSTICK},
	{"rstick", SDL_CONTROLLER_BUTTON_RIGHTSTICK},
	{"lshoulder", SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
	{"rshoulder", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},
	{"dpad_up", SDL_CONTROLLER_BUTTON_DPAD_UP},
	{"dpad_down", SDL_CONTROLLER_BUTTON_DPAD_DOWN},
	{"dpad_left", SDL_CONTROLLER_BUTTON_DPAD_LEFT},
	{"dpad_right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT}
};

std::unordered_map<std::string, int> Gamepad::string_to_axis = {
	{"axis_leftx", SDL_CONTROLLER_AXIS_LEFTX},
	{"axis_lefty", SDL_CONTROLLER_AXIS_LEFTY},
	{"axis_rightx", SDL_CONTROLLER_AXIS_RIGHTX},
	{"axis_righty", SDL_CONTROLLER_AXIS_RIGHTY},
	{"axis_triggerleft", SDL_CONTROLLER_AXIS_TRIGGERLEFT},
	{"axis_triggerright", SDL_CONTROLLER_AXIS_TRIGGERRIGHT}
};

};
