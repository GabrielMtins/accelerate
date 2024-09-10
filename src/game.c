#include "game.h"
#include "scene.h"
#include "testent.h"

#include <time.h>

#define STRING_SIZE 256

typedef struct{
	char str[STRING_SIZE];
} CustomStr;

static CustomStr CustomStr_Create(const char *str);
static size_t String_Hash(void *str, size_t n);
static void TexturePack_Destructor(void *ptr);
static void SfxPack_Destructor(void *ptr);

Game * Game_Create(const char *title, int internal_width, int internal_height){
	Game *game;
	srand(time(NULL));

	game = (Game *) malloc(sizeof(Game));

	game->context = Context_Create(title, internal_width, internal_height);

	game->texture_pack = CHash_Create(CustomStr, Texture *, MAX_TEXTURES);
	CHash_SetCustomHashFunction(game->texture_pack, String_Hash);

	game->sfx_pack = CHash_Create(CustomStr, Texture *, MAX_TEXTURES);
	CHash_SetCustomHashFunction(game->sfx_pack, String_Hash);

	game->current_scene = Scene_Create(game->context, game);

	Game_LoadTexture(game, "player.png", 32, 32);

	Scene_AddEntity(game->current_scene, Testent_Create(game->current_scene));
	Scene_AddEntity(game->current_scene, Testent2_Create(game->current_scene));
	
	return game;
}

void Game_Update(Game *game){
	Context_UpdateTime(game->context);

	Scene_Update(game->current_scene);
}

void Game_Render(Game *game){
	SDL_SetRenderDrawColor(game->context->renderer, 0x10, 0x10, 0x10, 0xff);
	SDL_RenderClear(game->context->renderer);

	Scene_Render(game->current_scene);

	SDL_RenderPresent(game->context->renderer);
}

void Game_Loop(Game *game){
	Context_PollEvent(game->context);
		
	Game_Update(game);
	Game_Render(game);
		
	Context_UpdateTime(game->context);
}

void Game_Run(Game *game){
	while(!game->context->quit){
		Game_Loop(game);
	}
}

int Game_LoadTexture(Game *game, const char *filename, int cell_width, int cell_height){
	if(game == NULL || filename == NULL) return 0;
	Texture *texture;
	CustomStr str;

	texture = Texture_Load(game->context, filename, cell_width, cell_height);
	str = CustomStr_Create(filename);

	return CHash_Insert(game->texture_pack, &str, &texture);
}

Texture * Game_GetTexture(Game *game, const char *filename){
	if(game == NULL || filename == NULL) return NULL;
	Texture *tmp;
	CustomStr str;

	str = CustomStr_Create(filename);

	if(CHash_Get(game->texture_pack, &str, &tmp))
		return tmp;

	return NULL;
}

int Game_LoadSfx(Game *game, const char *filename, int type){
	if(game == NULL || filename == NULL) return 0;

	Sfx *sfx;
	CustomStr str;

	if(type == SFX_SFX)
		sfx = Sfx_LoadSfx(filename);
	else
		sfx = Sfx_LoadMusic(filename);

	str = CustomStr_Create(filename);

	return CHash_Insert(game->sfx_pack, &str, &sfx);
}

Sfx * Game_GetSfx(Game *game, const char *filename){
	if(game == NULL || filename == NULL) return NULL;
	Sfx *tmp;
	CustomStr str;

	str = CustomStr_Create(filename);

	if(CHash_Get(game->sfx_pack, &str, &tmp))
		return tmp;

	return NULL;
}

void Game_Destroy(Game *game){
	CHash_ForEachValue(game->texture_pack, TexturePack_Destructor);
	CHash_Destroy(game->texture_pack);

	CHash_ForEachValue(game->sfx_pack, SfxPack_Destructor);
	CHash_Destroy(game->sfx_pack);

	Scene_Destroy(game->current_scene);
	Context_Destroy(game->context);

	free(game);
}

static CustomStr CustomStr_Create(const char *str){
	CustomStr new_str;

	memset(new_str.str, 0, STRING_SIZE);
	strcpy(new_str.str, str);

	return new_str;
}

static size_t String_Hash(void *ptr, size_t n){
	char *str = ((CustomStr *) ptr)->str;
	size_t hash = 0;
	(void) n;

	for(size_t i = 0; i < strlen(str); i++){
		hash += str[i];
		hash += hash << 10;
		hash ^= hash >> 6;
	}

	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;

	return hash;
}

static void TexturePack_Destructor(void *ptr){
	Texture **texture = (Texture **) ptr;

	Texture_Destroy(*texture);
}

static void SfxPack_Destructor(void *ptr){
	Sfx **sfx = (Sfx **) ptr;

	Sfx_Destroy(*sfx);
}
