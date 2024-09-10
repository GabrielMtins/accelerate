#include "context.h"

Context * Context_Create(const char *title, int internal_width, int internal_height){
	Context *context;

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	Mix_Init(MIX_INIT_MP3);
	
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
		fprintf(stderr, "Failed to init audio\n");
	}

	context = (Context *) malloc(sizeof(Context));

	context->internal_width = internal_width;
	context->internal_height = internal_height;

	context->window_width = context->internal_width * 2;
	context->window_height = context->internal_height * 2;

	context->window = SDL_CreateWindow(
			title,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			context->window_width,
			context->window_height,
			SDL_WINDOW_RESIZABLE
			);

	context->renderer = SDL_CreateRenderer(
			context->window,
			-1,
			0
			);

	SDL_RenderSetLogicalSize(context->renderer, context->internal_width, context->internal_height);

	SDL_SetRenderDrawBlendMode(context->renderer, SDL_BLENDMODE_BLEND);

	context->font = NULL;

	context->quit = 0;
	context->delta_time = 0;
	context->first_time = SDL_GetTicks64();

	return context;
}

void Context_GetMousePosition(Context *context, int *x, int *y){
	int mx, my, board_x, board_y;
	double scale = 0;

	board_x = 0;
	board_y = 0;

	if(context->window_width > context->internal_width * context->window_height / context->internal_height){
		board_x = context->window_width - context->internal_width * context->window_height / context->internal_height;
		board_x /= 2;

		scale = (double) context->internal_height / context->window_height;
	}
	else{
		board_y = context->window_height - context->internal_height * context->window_width / context->internal_width;
		board_y /= 2;

		scale = (double) context->internal_width / context->window_width;
	}

	SDL_GetMouseState(&mx, &my);

	mx -= board_x;
	my -= board_y;
	
	mx = scale * mx;
	my = scale * my;

	*x = mx;
	*y = my;
}

double Context_GetDeltaTime(Context *context){
	return context->delta_time;
}

void Context_PollEvent(Context *context){
	SDL_Event event;

	while(SDL_PollEvent(&event)){
		if(event.type == SDL_QUIT)
			context->quit = 1;
		else if(event.type == SDL_WINDOWEVENT){
			if(event.window.event == SDL_WINDOWEVENT_RESIZED){
				SDL_GetWindowSize(
						context->window,
						&context->window_width,
						&context->window_height
						);
			}
		}
	}
}

void Context_UpdateTime(Context *context){
	uint64_t new_time = SDL_GetTicks64();

	if(new_time - context->first_time < 1000 / FPS_MAX){
		SDL_Delay(1000 / FPS_MAX - new_time + context->first_time);
	}

	new_time = SDL_GetTicks();

	context->delta_time = 0.001 * (new_time - context->first_time);
	
	context->first_time = new_time;
}

uint64_t Context_GetTicks(Context *context){
	(void) context;

	return SDL_GetTicks64();
}

void Context_Destroy(Context *context){
	SDL_DestroyRenderer(context->renderer);
	SDL_DestroyWindow(context->window);

	if(context->font != NULL)
		TTF_CloseFont(context->font);

	Mix_CloseAudio();

	TTF_Quit();
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();

	free(context);
}
