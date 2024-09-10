#include "scene.h"
#include "quadtree.h"

static int RenderPackage_Compare(const void *p1, const void *p2);
static void Scene_UpdateEntities(Scene *scene);
static void Scene_SolveCollisions(Scene *scene);
static void Scene_UpdateRenderQueue(Scene *scene);
static void Scene_CallCollisionHandler(Scene *scene, Entity *myself, Entity *target);

Scene * Scene_Create(Context *context, Game *game){
	Scene *scene;

	scene = (Scene *) malloc(sizeof(Scene));

	scene->entities = CList_Create();
	scene->game = game;
	scene->context = context;

	scene->render_packages = CHeap_Create(RenderPackage, MAX_RENDER_PACKAGE, RenderPackage_Compare);

	return scene;
}

Texture * Scene_GetTexture(Scene *scene, const char *filename){
	return Game_GetTexture(scene->game, filename);
}

uint64_t Scene_GetTicks(Scene *scene){
	return Context_GetTicks(scene->context);
}

void Scene_SetCameraPosition(Scene *scene, Vec2 position){
	scene->camera_position = position;
}

void Scene_AddEntity(Scene *scene, Entity *entity){
	CList_PushBack(scene->entities, entity);
}

void Scene_Update(Scene *scene){
	Scene_UpdateEntities(scene);
	Scene_SolveCollisions(scene);
	Scene_UpdateRenderQueue(scene);
}

void Scene_Render(Scene *scene){
	RenderPackage package;

	while(!CHeap_IsEmpty(scene->render_packages)){
		CHeap_Remove(scene->render_packages, &package);

		Texture_RenderCell(
				scene->context,
				package.texture,
				package.position.x,
				package.position.y,
				package.id
				);
	}
}

void Scene_AddToRenderQueue(Scene *scene, RenderPackage *package){
	RenderPackage package_copy = *package;
	Rect clip_screen = {0, 0, scene->context->internal_width, scene->context->internal_height};
	Rect texture_rect;

	package_copy.position = Vec2_Sub(package_copy.position, scene->camera_position);

	texture_rect = Rect_CreateVec2(
			Vec2_Sub(package->position, scene->camera_position),
			Vec2_Create(package->texture->cell_width, package->texture->cell_height)
			);

	if(Rect_Intersects(texture_rect, clip_screen)){
		CHeap_Insert(scene->render_packages, &package_copy);
	}
}

CList * Scene_FindRadius(Scene *scene, Vec2 origin, double radius){
	CList *found_entities;
	Entity *current;

	found_entities = CList_Create();

	CList_ForEach(item, scene->entities){
		current = (Entity *) CItem_GetKey(item);

		if(Vec2_LengthSqr(Vec2_Sub(origin, current->position)) < radius * radius){
			CList_PushBack(found_entities, current);
		}
	}

	return found_entities;
}

CList * Scene_FindByClassname(Scene *scene, const char *classname){
	CList *found_entities;
	Entity *current;

	found_entities = CList_Create();

	CList_ForEach(item, scene->entities){
		current = (Entity *) CItem_GetKey(item);

		if(strcmp(current->classname, classname) == 0){
			CList_PushBack(found_entities, current);
		}
	}

	return found_entities;
}

void Scene_Destroy(Scene *scene){
	CHeap_Destroy(scene->render_packages);

	CList_ForEach(item, scene->entities){
		Entity_Destroy(scene, CItem_GetKey(item));
	}

	CList_Destroy(scene->entities);

	free(scene);
}

static int RenderPackage_Compare(const void *p1, const void *p2){
	RenderPackage *a, *b;

	a = (RenderPackage *) p1;
	b = (RenderPackage *) p2;

	/* if they are in the same layer, sort by y position */
	if(a->layer == b->layer)
		return b->position.y + b->texture->cell_height - a->position.y - a->texture->cell_height;

	/* otherwise, sort by layer */
	return b->layer - a->layer;
}

static void Scene_UpdateEntities(Scene *scene){
	Entity *current_entity;

	CList_ForEach(item, scene->entities){
		current_entity = (Entity *) CItem_GetKey(item);

		Entity_Update(scene, current_entity);
	}
}

static void Scene_SolveCollisions(Scene *scene){
	Entity *myself, *target;

	/* I literally have a QuadTree built up for this, but
	 * for prototiping i'll just use a O(n^2) algorithm.
	 * Remember to implement the QuadTree later */
	CList_ForEach(i, scene->entities){
		myself = (Entity *) CItem_GetKey(i);

		CList_ForEach(j, scene->entities){
			target = (Entity *) CItem_GetKey(j);

			if(myself == target) continue;

			/* myself will collide with the target */
			if(Entity_CheckCollision(myself, target) && ((myself->collision_mask & target->collision_layer) != 0)){
				Scene_CallCollisionHandler(scene, myself, target);
			}
		}
	}
}

static void Scene_UpdateRenderQueue(Scene *scene){
	Entity *current_entity;

	CList_ForEach(item, scene->entities){
		current_entity = (Entity *) CItem_GetKey(item);

		Entity_Render(scene, current_entity);
	}
}

static void Scene_CallCollisionHandler(Scene *scene, Entity *myself, Entity *target){
	if(myself->collisionHandler != NULL)
		myself->collisionHandler(scene, myself, target);

	if(target->collisionHandler != NULL)
		target->collisionHandler(scene, target, myself);

	/* myself is the entity who causes the collision. target is the one who
	 * gets collided.
	 * since myself is the one who caused the collision, it is the one who
	 * gets pushed. use this behaviour carefully.
	 * for example, if a entity "player" is on collision layer 1 and its collision mask is
	 * layer 2. if it collides with an object on
	 * collision layer 2, it's the player who gets pushed.
	 * But if player is on collision layer 1 and the other object "box" has its collision mask
	 * at layer 1, when the box collides with the player it's the box who gets pushed */

	/* Since this kind of behaviour can't be enforced when "myself" is a static body,
	 * because a static body doesn't move, the collision is solved only if "myself"
	 * is a dynamic body */

	if(myself->body_type == BODY_TYPE_DYNAMIC && (target->body_type != BODY_TYPE_CHARACTER))
		Entity_SolveCollision(myself, target);
}

