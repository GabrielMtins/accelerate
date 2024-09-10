#ifndef SCENE_H
#define SCENE_H

#include "datas/carray.h"
#include "context.h"
#include "global.h"
#include "vec2.h"
#include "texture.h"
#include "entity.h"

#define Scene_AddComponentArray(scene, type) Scene_GetComponentArrayStr(scene, #type, sizeof(type))
#define Scene_GetComponentArray(scene, type) Scene_GetComponentArrayStr(scene, #type)

typedef struct{
	Texture *texture;
	Vec2 position;
	int id;
	int layer;
} RenderPackage;

Scene * Scene_Create(Context *context, Game *game);

int Scene_AddSystemHandler(Scene *scene, int (*system_update)(Scene *));

void Scene_UpdateSystems(Scene *scene);

int Scene_AddComponentArrayStr(Scene *scene, const char *component_name, size_t component_size);

CArray * Scene_GetComponentArrayStr(Scene *scene, const char *component_name);

Texture * Scene_GetTexture(Scene *scene, const char *filename);

uint64_t Scene_GetTicks(Scene *scene);

void Scene_SetCameraPosition(Scene *scene, Vec2 position);

void Scene_AddEntity(Scene *scene, Entity *entity);

void Scene_Update(Scene *scene);

void Scene_Render(Scene *scene);

void Scene_AddToRenderQueue(Scene *scene, RenderPackage *package);

CList * Scene_FindRadius(Scene *scene, Vec2 origin, double radius);

CList * Scene_FindByClassname(Scene *scene, const char *classname);

void Scene_Destroy(Scene *scene);

#endif
