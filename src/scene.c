#include "scene.h"
#include "hashstr.h"
#include "game.h"

#include "TransformComponent.h"
#include "SpriteComponent.h"

typedef struct{
	int (*system_update)(Scene *);
} SystemHandler;

static int SpriteId_Compare(const void *a, const void *b);
static int SpriteSystem_Update(Scene *scene);

void * Entity_GetComponentStr(Scene *scene, size_t id, const char *type){
	CArray *arr;

	arr = Scene_GetComponentArrayStr(scene, type);

	if(arr == NULL) return NULL;

	return CArray_GetReference(arr, id);
}

Scene * Scene_Create(Context *context, Game *game){
	Scene *scene;

	scene = (Scene *) malloc(sizeof(Scene));

	scene->game = game;
	scene->context = context;

	scene->components_arrays = CHash_Create(HashStr, CArray *, MAX_COMPONENTS_ARRAYS);
	scene->next_entity = CHeap_Create(size_t, 128, SpriteId_Compare);
	scene->systems = CList_Create();
	scene->max_entity = 0;

	Scene_AddSystemHandler(scene, SpriteSystem_Update);

	return scene;
}

size_t Scene_AddEntity(Scene *scene){
	size_t id;

	if(CHeap_IsEmpty(scene->next_entity)){
		return scene->max_entity++;
	}

	CHeap_GetTop(scene->next_entity, &id);

	return id;
}

int Scene_AddSystemHandler(Scene *scene, int (*system_update)(Scene *)){
	SystemHandler *handler;

	handler = (SystemHandler *) malloc(sizeof(SystemHandler));

	handler->system_update = system_update;

	CList_PushBack(scene->systems, handler);

	return 1;
}

void Scene_UpdateSystems(Scene *scene){
	SystemHandler *handler;

	CList_ForEach(item, scene->systems){
		handler = (SystemHandler *) CItem_GetKey(item);

		handler->system_update(scene);
	}
}

int Scene_AddComponentArrayStr(Scene *scene, const char *component_name, size_t component_size){
	CArray *arr = CArray_CreateBytes(component_size, MAX_COMPONENTS_PER_ARRAY);
	HashStr str = HashStr_Create(component_name);

	return CHash_Insert(scene->components_arrays, &str, &arr);
}

CArray * Scene_GetComponentArrayStr(Scene *scene, const char *component_name){
	CArray *arr;
	HashStr str = HashStr_Create(component_name);

	if(CHash_Get(scene->components_arrays, &str, &arr))
		return arr;

	return NULL;
}

Texture * Scene_GetTexture(Scene *scene, const char *filename){
	return Game_GetTexture(scene->game, filename);
}

uint64_t Scene_GetTicks(Scene *scene){
	return Context_GetTicks(scene->context);
}

void Scene_Update(Scene *scene){
	Scene_UpdateSystems(scene);
}

void Scene_Render(Scene *scene){
}

void Scene_Destroy(Scene *scene){
	free(scene);
}

static int SpriteId_Compare(const void *a, const void *b){
	int *x, *y;

	x = (int *) a;
	y = (int *) b;

	return y - x;
}

static int SpriteSystem_Update(Scene *scene){
	CArray *transform_array, *sprite_array;

	TransformComponent *transform;
	SpriteComponent *sprite;

	transform_array = Scene_GetComponentArray(scene, TransformComponent);
	sprite_array = Scene_GetComponentArray(scene, SpriteComponent);

	for(size_t i = 0; i < scene->max_entity; i++){
		transform = CArray_GetReference(transform_array, i);
		sprite = CArray_GetReference(sprite_array, i);

		if(!sprite->used)
			continue;

		Texture_RenderCell(
				scene->context,
				sprite->texture,
				sprite->position.x,
				sprite->position.y,
				sprite->id
				);
	}

	return 1;
}
