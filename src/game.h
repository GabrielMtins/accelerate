#ifndef GAME_H
#define GAME_H

#include "context.h"
#include "global.h"
#include "texture.h"
#include "sfx.h"

#include "datas/carray.h"

Game * Game_Create(const char *title, int internal_width, int internal_height);

void Game_Update(Game *game);

void Game_Render(Game *game);

void Game_Loop(Game *game);

void Game_Run(Game *game);

int Game_LoadTexture(Game *game, const char *filename, int cell_width, int cell_height);

Texture * Game_GetTexture(Game *game, const char *filename);

int Game_LoadSfx(Game *game, const char *filename, int type);

Sfx * Game_GetSfx(Game *game, const char *filename);

void Game_Destroy(Game *game);

#endif
