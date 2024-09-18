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

	this->font = NULL;

	this->quit = false;
	this->delta_time = 0;
	this->first_time = SDL_GetTicks64();
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
	return delta_time;
}

void Context::pollEvent(void){
	SDL_Event event;

	while(SDL_PollEvent(&event)){
		if(event.type == SDL_QUIT)
			quit = true;
		else if(event.type == SDL_WINDOWEVENT){
			if(event.window.event == SDL_WINDOWEVENT_RESIZED){
				SDL_GetWindowSize(
						window,
						&window_width,
						&window_height
						);
			}
		}
	}
}

void Context::updateTime(void){
	uint64_t new_time = SDL_GetTicks64();

	if(new_time - first_time < 1000 / FPS_MAX){
		SDL_Delay(1000 / FPS_MAX - new_time + first_time);
	}

	new_time = SDL_GetTicks64();

	delta_time = 0.001 * (new_time - first_time);
	
	first_time = new_time;
}

uint64_t Context::getTicks(void){
	return first_time;
}

Context::~Context(void){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	if(font != NULL)
		TTF_CloseFont(font);

	Mix_CloseAudio();

	TTF_Quit();
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}

void Context::clearScreen(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderClear(renderer);
}

void Context::renderPresent(void){
	SDL_RenderPresent(renderer);
}

SDL_Renderer * Context::getRenderer(void){
	return renderer;
}

};
