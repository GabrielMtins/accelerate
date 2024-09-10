#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define FPS_MAX 60

/* Contexto para renderização do jogo
 * Estarei usando o renderer do SDL2 */

typedef struct{
	SDL_Window *window;
	SDL_Renderer *renderer;
	TTF_Font *font;

	double delta_time;
	uint64_t first_time;
	int quit;

	int window_width, window_height;

	int internal_width, internal_height;
} Context;

/* Cria um objeto para contexto */
Context * Context_Create(const char *title, int internal_width, int internal_height);

/* Lê a posição atual do mouse na tela */
void Context_GetMousePosition(Context *context, int *x, int *y);

double Context_GetDeltaTime(Context *context);

/* Atualiza a leitura de eventos */
void Context_PollEvent(Context *context);

/* Atualiza o delta time interno */
void Context_UpdateTime(Context *context);

uint64_t Context_GetTicks(Context *context);

/* Libera a memória utilizada pelo contexto */
void Context_Destroy(Context *context);

#endif
