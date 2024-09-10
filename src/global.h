#ifndef GLOBAL_H
#define GLOBAL_H

/* Default structs used */

#include "context.h"
#include "vec2.h"
#include "datas/carray.h"
#include "datas/clist.h"
#include "datas/cheap.h"
#include "datas/chash.h"

#define MAX_COMPONENTS_PER_ARRAY 1024
#define MAX_COMPONENTS_ARRAYS 256
#define MAX_TEXTURES 256
#define MAX_SFX 256
#define MAX_RENDER_PACKAGE 4096
#define NUM_PHYSICS_LAYERS 20
#define WORLD_SIZE 256

#define LAYER_MASK(x) (1 << (x))

typedef struct game_s Game;
typedef struct scene_s Scene;

struct game_s{
	Context *context;
	CHash *texture_pack;
	CHash *sfx_pack;
	Scene *current_scene;
};

struct scene_s{
	Context *context;
	Game *game;
	CList *entities;
	CHeap *render_packages;
	Vec2 camera_position;

	CHash *components_arrays;
	CList *systems;
};

#endif
