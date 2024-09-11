#ifndef SCENE_H
#define SCENE_H

#include "datas/carray.h"
#include "context.h"
#include "global.h"
#include "vec2.h"
#include "texture.h"

#define Entity_GetComponent(scene, id, type) Entity_GetComponentStr(scene, id, #type)
#define Scene_AddComponentArray(scene, type) Scene_AddComponentArrayStr(scene, #type, sizeof(type))
#define Scene_GetComponentArray(scene, type) Scene_GetComponentArrayStr(scene, #type)

void * Entity_GetComponentStr(Scene *scene, size_t id, const char *type);

Scene * Scene_Create(Context *context, Game *game);

size_t Scene_AddEntity(Scene *scene);

int Scene_AddSystemHandler(Scene *scene, int (*system_update)(Scene *));

void Scene_UpdateSystems(Scene *scene);

int Scene_AddComponentArrayStr(Scene *scene, const char *component_name, size_t component_size);

CArray * Scene_GetComponentArrayStr(Scene *scene, const char *component_name);

Texture * Scene_GetTexture(Scene *scene, const char *filename);

uint64_t Scene_GetTicks(Scene *scene);

void Scene_Update(Scene *scene);

void Scene_Render(Scene *scene);

CList * Scene_FindRadius(Scene *scene, Vec2 origin, double radius);

CList * Scene_FindByClassname(Scene *scene, const char *classname);

void Scene_Destroy(Scene *scene);

#endif
