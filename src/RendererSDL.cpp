#include "RendererSDL.hpp"

namespace acc {

RendererSDL::RendererSDL(Context *context){
	renderer = SDL_CreateRenderer(
			context->getWindow(),
			-1,
			0
			);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	setScaling(RENDERER_SCALING_BESTFIT);
}

void RendererSDL::setWindowSize(int width, int height){
	window_width = width;
	window_height = height;
}

void RendererSDL::setInternalSize(int width, int height){
	if(framebuffer != NULL)
		SDL_DestroyTexture(framebuffer);

	framebuffer = SDL_CreateTexture(
			renderer,
			(SDL_PixelFormatEnum) SDL_PIXELFORMAT_RGBA8888,
			(SDL_TextureAccess) SDL_TEXTUREACCESS_TARGET,
			width,
			height
			);

	internal_width = width;
	internal_height = height;
}

void RendererSDL::setScaling(RendererScaling scaling){
	this->scaling = scaling;
}

void RendererSDL::clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderClear(renderer);

	SDL_SetRenderTarget(renderer, framebuffer);
}

void RendererSDL::renderPresent(void){
	SDL_SetRenderTarget(renderer, NULL);
	
	switch(scaling){
		case RENDERER_SCALING_EXPAND:
			SDL_RenderCopy(renderer, framebuffer, NULL, NULL);
			break;

		case RENDERER_SCALING_PIXELPERFECT:
			renderPixelPerfect();
			break;

		case RENDERER_SCALING_BESTFIT:
			renderBestFit();
			break;
	}

	SDL_RenderPresent(renderer);
}

RendererSDL::~RendererSDL(){
	SDL_DestroyTexture(framebuffer);
	SDL_DestroyRenderer(renderer);
}

void RendererSDL::renderPixelPerfect(void){
	int scale_factor = std::min(window_width / internal_width, window_height / internal_height);

	int defacto_width = scale_factor * internal_width;
	int defacto_height = scale_factor * internal_height;

	SDL_Rect defacto_rect = {
		(window_width - defacto_width) / 2,
		(window_height - defacto_height) / 2,
		defacto_width,
		defacto_height
	};

	SDL_RenderCopy(renderer, framebuffer, NULL, &defacto_rect);
}

void RendererSDL::renderBestFit(void){
	int defacto_width, defacto_height;

	if(window_width * internal_height < window_height * internal_width){
		defacto_width = window_width;
		defacto_height = internal_height * window_width / internal_width;
	}
	else{
		defacto_width = internal_width * window_height / internal_height;
		defacto_height = window_height;
	}

	SDL_Rect defacto_rect = {
		(window_width - defacto_width) / 2,
		(window_height - defacto_height) / 2,
		defacto_width,
		defacto_height
	};

	SDL_RenderCopy(renderer, framebuffer, NULL, &defacto_rect);
}

};

