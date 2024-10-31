#include "Context.hpp"

namespace acc {

Context::Context(const char *title, int internal_width, int internal_height){
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	Mix_Init(MIX_INIT_MP3);
	
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
		fprintf(stderr, "Failed to init audio\n");
	}

	this->internal_width = internal_width;
	this->internal_height = internal_height;

	this->window_width = internal_width * 2;
	this->window_height = internal_height * 2;

	this->window = SDL_CreateWindow(
			title,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			this->window_width,
			this->window_height,
			SDL_WINDOW_RESIZABLE
			);

	this->renderer = SDL_CreateRenderer(
			this->window,
			-1,
			0
			);

	SDL_RenderSetLogicalSize(this->renderer, this->internal_width, this->internal_height);

	SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);

	this->quit = false;
	this->delta_time = 0;
	this->first_time = SDL_GetTicks64();

	setUpKeys();
	setFps(165);
	minimum_delta = 1000;
}

void Context::close(void){
	quit = true;
}

void Context::setMinimumFps(uint32_t fps){
	minimum_delta = 1.0f / (float) fps;
}

void Context::setFps(uint32_t fps){
	this->fps = fps;
}

void Context::setFullscreenMode(void){
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void Context::setWindowedMode(void){
	SDL_SetWindowFullscreen(window, 0);
}

void Context::setWindowSize(int width, int height){
	SDL_SetWindowSize(window, width, height);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void Context::maximizeWindow(void){
	SDL_MaximizeWindow(window);
}

void Context::showCursor(bool enable){
	if(enable)
		SDL_ShowCursor(1);
	else
		SDL_ShowCursor(0);
}

int Context::getWidth(void){
	return internal_width;
}

int Context::getHeight(void){
	return internal_height;
}

bool Context::isRunning(void){
	return !quit;
}

float Context::getDeltaTime(void){
	if(minimum_delta < delta_time){
		return minimum_delta;
	}

	return delta_time;
}

void Context::pollEvent(void){
	SDL_Event event;

	while(SDL_PollEvent(&event)){
		if(event.type == SDL_QUIT){
			quit = true;
		}
		else if(event.type == SDL_WINDOWEVENT){
			if(event.window.event == SDL_WINDOWEVENT_RESIZED){
				SDL_GetWindowSize(
						window,
						&window_width,
						&window_height
						);
			}
		}
		else if(event.type == SDL_TEXTINPUT){
			text_input = event.text.text;
		}
	}

	/* update key state */
	const uint8_t *keys_pressed_on_frame = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < SDL_NUM_SCANCODES; i++){
		if(keys_pressed_on_frame[i]){
			if(key_state[i] == false){
				key_tick_pressed[i] = getTicks();
			}

			key_state[i] = true;
		}
		else{
			if(key_state[i] == true){
				key_tick_released[i] = getTicks();
			}

			key_state[i] = false;
		}
	}

	/* update mouse state */
	int mouse_state = SDL_GetMouseState(NULL, NULL);

	if(mouse_state & SDL_BUTTON(1)){
		if(mouse_button_state["left"] == false){
			mouse_button_tick_pressed["left"] = getTicks();
		}

		mouse_button_state["left"] = true;
	}
	else{
		if(mouse_button_state["left"])
			mouse_button_tick_released["left"] = getTicks();

		mouse_button_state["left"] = false;
	}

	if(mouse_state & SDL_BUTTON(2)){
		if(mouse_button_state["middle"] == false){
			mouse_button_tick_pressed["middle"] = getTicks();
		}

		mouse_button_state["middle"] = true;
	}
	else{
		if(mouse_button_state["middle"])
			mouse_button_tick_released["middle"] = getTicks();

		mouse_button_state["middle"] = false;
	}

	if(mouse_state & SDL_BUTTON(3)){
		if(mouse_button_state["right"] == false){
			mouse_button_tick_pressed["right"] = getTicks();
		}

		mouse_button_state["right"] = true;
	}
	else{
		if(mouse_button_state["right"])
			mouse_button_tick_released["right"] = getTicks();

		mouse_button_state["right"] = false;
	}
}

void Context::updateTime(void){
	uint64_t new_time = SDL_GetTicks64();

	if(new_time - first_time < 1000 / fps){
		SDL_Delay(1000 / fps - new_time + first_time);
	}

	new_time = SDL_GetTicks64();

	delta_time = 0.001 * (new_time - first_time);
	
	first_time = new_time;
}

uint64_t Context::getTicks(void){
	return first_time;
}

void Context::clearScreen(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderClear(renderer);
}

void Context::renderPresent(void){
	SDL_RenderPresent(renderer);
}

bool Context::getKey(const std::string& key){
	if(string_to_keys.find(key) == string_to_keys.end()) return false;

	int index = string_to_keys[key];

	return key_state[index];
}

bool Context::getKeyDown(const std::string& key){
	if(string_to_keys.find(key) == string_to_keys.end()) return false;
	
	int index = string_to_keys[key];

	return key_state[index] && (key_tick_pressed[index] == getTicks());
}

bool Context::getKeyUp(const std::string& key){
	if(string_to_keys.find(key) == string_to_keys.end()) return false;
	
	int index = string_to_keys[key];

	return !key_state[index] && (key_tick_released[index] == getTicks());
}

std::unordered_map<std::string, int>& Context::getStringToKeys(void){
	return string_to_keys;
}

std::string Context::getTextInput(void){
	return text_input;
}

void Context::getMousePosition(int *x, int *y){
	int mx, my, board_x, board_y;
	double scale = 0;

	board_x = 0;
	board_y = 0;

	if(window_width > internal_width * window_height / internal_height){
		board_x = window_width - internal_width * window_height / internal_height;
		board_x /= 2;

		scale = (double) internal_height / window_height;
	}
	else{
		board_y = window_height - internal_height * window_width / internal_width;
		board_y /= 2;

		scale = (double) internal_width / window_width;
	}

	SDL_GetMouseState(&mx, &my);

	mx -= board_x;
	my -= board_y;
	
	mx = scale * mx;
	my = scale * my;

	*x = mx;
	*y = my;
}

bool Context::getMouseButton(const std::string& mouse_button){
	if(mouse_button_state.find(mouse_button) == mouse_button_state.end())
		return false;

	return mouse_button_state[mouse_button];
}

bool Context::getMouseButtonDown(const std::string& mouse_button){
	if(mouse_button_state.find(mouse_button) == mouse_button_state.end())
		return false;

	return mouse_button_state[mouse_button] && (mouse_button_tick_pressed[mouse_button] == getTicks());
}

bool Context::getMouseButtonUp(const std::string& mouse_button){
	if(mouse_button_state.find(mouse_button) == mouse_button_state.end())
		return false;

	return !mouse_button_state[mouse_button] && (mouse_button_tick_released[mouse_button] == getTicks());
}

SDL_Renderer * Context::getRenderer(void){
	return renderer;
}

Context::~Context(void){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	Mix_CloseAudio();

	TTF_Quit();
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}

void Context::setUpKeys(void){
	/* set up letters */
	for(int i = 0; i < 26; i++){
		std::string letter;
		letter += (char) ((int) 'a' + i);
		string_to_keys[letter] = SDL_SCANCODE_A + i;

		letter = "";
		letter += (char) ((int) 'A' + i);
		string_to_keys[letter] = SDL_SCANCODE_A + i;
	}

	/* set up numbers */
	for(int i = 0; i < 10; i++){
		std::string number;
		number += (char) ((int) '0' + i);
		string_to_keys[number] = SDL_SCANCODE_1 + i;
	}

	/* set up function keys */
	for(int i = 1; i <= 12; i++){
		std::string fkey = "f";
		fkey += std::to_string(i);
		string_to_keys[fkey] = SDL_SCANCODE_F1 + i - 1;

		fkey = "F";
		fkey += std::to_string(i);
		string_to_keys[fkey] = SDL_SCANCODE_F1 + i - 1;
	}

	string_to_keys["return"] = SDL_SCANCODE_RETURN;
	string_to_keys["escape"] = SDL_SCANCODE_ESCAPE;
	string_to_keys["backspace"] = SDL_SCANCODE_BACKSPACE;
	string_to_keys["tab"] = SDL_SCANCODE_TAB;
	string_to_keys["space"] = SDL_SCANCODE_SPACE;

	string_to_keys["lctrl"] = SDL_SCANCODE_LCTRL;
	string_to_keys["lshift"] = SDL_SCANCODE_LSHIFT;
	string_to_keys["lalt"] = SDL_SCANCODE_LALT;

	string_to_keys["rctrl"] = SDL_SCANCODE_RCTRL;
	string_to_keys["rshift"] = SDL_SCANCODE_RSHIFT;
	string_to_keys["ralt"] = SDL_SCANCODE_RALT;

	string_to_keys["left"] = SDL_SCANCODE_LEFT;
	string_to_keys["up"] = SDL_SCANCODE_UP;
	string_to_keys["down"] = SDL_SCANCODE_DOWN;
	string_to_keys["right"] = SDL_SCANCODE_RIGHT;

	for(int i = 0; i < SDL_NUM_SCANCODES; i++){
		key_state[i] = false;
		key_tick_pressed[i] = 0;
		key_tick_released[i] = 0;
	}

	mouse_button_state["left"] = false;
	mouse_button_state["right"] = false;
	mouse_button_state["middle"] = false;

	mouse_button_tick_pressed["left"] = 0;
	mouse_button_tick_pressed["right"] = 0;
	mouse_button_tick_pressed["middle"] = 0;

	mouse_button_tick_released["left"] = 0;
	mouse_button_tick_released["right"] = 0;
	mouse_button_tick_released["middle"] = 0;
}

};
