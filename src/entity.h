#ifndef ENTITY_H
#define ENTITY_H

#include "global.h"
#include "vec2.h"
#include "game.h"
#include "rect.h"

#define ENTITY_NONE -1

/* Body Types:
 * Each of them calls the collisionHandler function pointer
 * # BODY_TYPE_STATIC:
 * 		- participates in collision detection
 * 		- does not move when collided
 * # BODY_TYPE_DYNAMIC:
 * 		- participates in collision detection
 * 		- does move when collided 
 * # BODY_TYPE_CHARACTER:
 * 		- can detect collision but it'll not move itself
 * 		or other entities
 */

enum BODY_TYPES{
	BODY_TYPE_STATIC = 0,
	BODY_TYPE_DYNAMIC,
	BODY_TYPE_CHARACTER
};

struct entity_s{
	Vec2 position;
	Vec2 velocity;
	Texture *texture; /* customizable, default: NULL */

	Vec2 hitbox_size; /* customizable */
	Vec2 hitbox_offset; /* customizable, default: (0, 0) */

	int texture_id; /* customizable, default: -1 */
	int is_dead;
	int health; /* customizable, default: 100 */
	int draw_layer; /* customizable, default: 0*/
	uint32_t collision_layer; /* customizable, default: LAYER_MASK(1) */
	uint32_t collision_mask; /* customizable, default: LAYER_MASK(1) */
	char *classname;
	int light_when_damaged;
	int was_damaged;

	double damage_timer;
	double damage_cooldown; /* customizable, default: 0 */

	void *properties; /* customizable, default: NULL */

	uint64_t next_think; /* customizable */

	uint8_t body_type; /* customizable */
	
	struct entity_s *target; /* customizable, default: NULL */

	void (*think)(Scene *, struct entity_s *); /* customizable */
	void (*collisionHandler)(Scene *, struct entity_s *, struct entity_s *); /* customizable */
	/* the first argument is the scene;
	 * second entity argument from collision handler is the entity itself;
	 * the thid one is the entity who collided; */
};

typedef struct entity_s Entity;

Entity * Entity_Create(Scene *scene);

void Entity_SetClassname(Entity *entity, const char *classname);

void Entity_Update(Scene *scene, Entity *entity);

void Entity_Render(Scene *scene, Entity *entity);

Rect Entity_GetRectHitbox(Entity *entity);

int Entity_CheckCollision(Entity *e1, Entity *e2);

// Only e1 will move
void Entity_SolveCollision(Entity *e1, Entity *e2);

// Returns 1 if the entity got damaged
int Entity_DoDamage(Entity *entity, int damage);

/* Sets the entity with a velocity with modulus velocity and
 * a random direction */
void Entity_RandomDirection(Entity *entity, double velocity);

Vec2 Entity_GetHitboxCenter(Entity *entity);

double Entity_DistSqrBetween(Entity *a, Entity *b);

void Entity_Destroy(Scene *game, Entity *entity);

#endif
