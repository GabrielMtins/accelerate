#include "entity.h"
#include "scene.h"

#include <math.h>

#define DAMAGE_COOLDOWN_BLINK 0.1

Entity * Entity_Create(Scene *scene){
	Entity *entity = (Entity *) malloc(sizeof(Entity));
	
	entity->position = Vec2_Create(0, 0);
	entity->velocity = Vec2_Create(0, 0);
	entity->hitbox_size = Vec2_Create(24, 24);
	entity->hitbox_offset = Vec2_Create(0, 0);

	entity->health = 100;

	entity->classname = NULL;
	entity->texture = NULL;
	entity->texture_id = -1;
	entity->next_think = 0;

	entity->think = NULL;
	entity->collisionHandler = NULL;

	entity->draw_layer = 0;
	entity->collision_mask = LAYER_MASK(1);
	entity->collision_layer = LAYER_MASK(1);
	entity->is_dead = 0;
	entity->light_when_damaged = 1;
	entity->was_damaged = 0;

	entity->body_type = BODY_TYPE_DYNAMIC;

	entity->damage_timer = 100;
	entity->damage_cooldown = 0.1;

	entity->properties = NULL;

	return entity;
}

void Entity_SetClassname(Entity *entity, const char *classname){
	if(entity->classname != NULL)
		free(entity->classname);

	if(classname != NULL){
		entity->classname = (char *) malloc(strlen(classname) + 1);
		strcpy(entity->classname, classname);
	}
	else
		entity->classname = NULL;
}

void Entity_Update(Scene *scene, Entity *entity){
	entity->position = Vec2_Add(
			entity->position,
			Vec2_Mul(entity->velocity, Context_GetDeltaTime(scene->context))
			);

	if(Scene_GetTicks(scene) > entity->next_think && entity->think != NULL){
		entity->next_think = Scene_GetTicks(scene);
		entity->think(scene, entity);
	}

	if(entity->health <= 0) entity->is_dead = 1;
}

void Entity_Render(Scene *scene, Entity *entity){
	RenderPackage package = {
		entity->texture,
		entity->position,
		entity->texture_id,
		entity->draw_layer
	};

	Scene_AddToRenderQueue(scene, &package);
}

Rect Entity_GetRectHitbox(Entity *entity){
	return
		Rect_CreateVec2(
				Vec2_Add(entity->position, entity->hitbox_offset),
				entity->hitbox_size
				);
}

int Entity_CheckCollision(Entity *e1, Entity *e2){
	Vec2 pos1 = Vec2_Add(e1->position, e1->hitbox_offset);
	Vec2 pos2 = Vec2_Add(e2->position, e2->hitbox_offset);

	if(pos1.x + e1->hitbox_size.x <= pos2.x) return 0;
	if(pos1.y + e1->hitbox_size.y <= pos2.y) return 0;
	if(pos1.x > pos2.x + e2->hitbox_size.x) return 0;
	if(pos1.y > pos2.y + e2->hitbox_size.y) return 0;

	return 1;
}

void Entity_SolveCollision(Entity *e1, Entity *e2){
	Vec2 pos1 = Vec2_Add(e1->position, e1->hitbox_offset);
	Vec2 pos2 = Vec2_Add(e2->position, e2->hitbox_offset);

	if(!Entity_CheckCollision(e1, e2)) return;

	double dx =
		fmin(pos1.x + e1->hitbox_size.x, pos2.x + e2->hitbox_size.x)
		-
		fmax(pos1.x, pos2.x);

	double dy =
		fmin(pos1.y + e1->hitbox_size.y, pos2.y + e2->hitbox_size.y)
		-
		fmax(pos1.y, pos2.y);

	if(fabs(dx) < fabs(dy)){
		if(pos1.x < pos2.x)
			e1->position.x -= dx;
		else
			e1->position.x += dx;
	}
	else{
		if(pos1.y < pos2.y)
			e1->position.y -= dy;
		else
			e1->position.y += dy - 1;
	}
}

int Entity_DoDamage(Entity *entity, int damage){
	if(entity->light_when_damaged || entity->damage_cooldown == 0 || entity->damage_timer >= entity->damage_cooldown){
		entity->health -= damage;
		entity->damage_timer = 0;
		entity->was_damaged = 1;

		return 1;
	}

	return 0;
}

void Entity_RandomDirection(Entity *entity, double velocity){
	Vec2 random_vector = Vec2_Create(
			(double) (rand() % 101 - 50) / 50,
			(double) (rand() % 101 - 50) / 50
			);

	random_vector = Vec2_Normalize(random_vector);

	entity->velocity = Vec2_Mul(random_vector, velocity);
}

Vec2 Entity_GetHitboxCenter(Entity *entity){
	Vec2 center = Vec2_Add(entity->position, entity->hitbox_offset);
	
	center = Vec2_Add(
			center,
			Vec2_Mul(entity->hitbox_size, 0.5)
			);

	return center;
}

double Entity_DistSqrBetween(Entity *a, Entity *b){
	return Vec2_LengthSqr(Vec2_Sub(
			Entity_GetHitboxCenter(a),
			Entity_GetHitboxCenter(b)
			));
}

void Entity_Destroy(Scene *scene, Entity *entity){
	if(entity == NULL) return;
	
	if(entity->classname != NULL)
		free(entity->classname);

	if(entity->properties != NULL){
		free(entity->properties);
	}

	free(entity);
}
