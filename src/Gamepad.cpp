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
	{"A", SDL_CONTROLLER_BUTTON_A},
	{"B", SDL_CONTROLLER_BUTTON_A},
	{"X", SDL_CONTROLLER_BUTTON_X},
	{"Y", SDL_CONTROLLER_BUTTON_Y},
	{"BACK", SDL_CONTROLLER_BUTTON_BACK},
	{"GUIDE", SDL_CONTROLLER_BUTTON_GUIDE},
	{"START", SDL_CONTROLLER_BUTTON_START},
	{"LSTICK", SDL_CONTROLLER_BUTTON_LEFTSTICK},
	{"RSTICK", SDL_CONTROLLER_BUTTON_RIGHTSTICK},
	{"LSHOULDER", SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
	{"RSHOULDER", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},
	{"DPAD_UP", SDL_CONTROLLER_BUTTON_DPAD_UP},
	{"DPAD_DOWN", SDL_CONTROLLER_BUTTON_DPAD_DOWN},
	{"DPAD_LEFT", SDL_CONTROLLER_BUTTON_DPAD_LEFT},
	{"DPAD_RIGHT", SDL_CONTROLLER_BUTTON_DPAD_RIGHT},
	{"AXIS_LEFTX", SDL_CONTROLLER_AXIS_LEFTX},
	{"AXIS_LEFTY", SDL_CONTROLLER_AXIS_LEFTY},
	{"AXIS_RIGHTX", SDL_CONTROLLER_AXIS_RIGHTX},
	{"AXIS_RIGHTY", SDL_CONTROLLER_AXIS_RIGHTY},
	{"AXIS_TRIGGERLEFT", SDL_CONTROLLER_AXIS_TRIGGERLEFT},
	{"AXIS_TRIGGERRIGHT", SDL_CONTROLLER_AXIS_TRIGGERRIGHT},
};

};
