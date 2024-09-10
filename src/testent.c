#include "testent.h"
#include "scene.h"

Entity * Testent_Create(Scene *scene){
	Entity *entity;

	entity = Entity_Create(scene);
	Entity_SetClassname(entity, "classname1");

	entity->texture = Scene_GetTexture(scene, "player.png");
	entity->hitbox_size = Vec2_Create(32, 32);

	entity->texture_id = 0;

	entity->think = Testent_Think;

	entity->collision_layer = LAYER_MASK(1);
	entity->collision_mask = LAYER_MASK(1);

	return entity;
}

void Testent_Think(Scene *scene, Entity *entity){
	const uint8_t *keys = SDL_GetKeyboardState(NULL);

	Scene_SetCameraPosition(
			scene,
			Vec2_Sub(entity->position, Vec2_Create(scene->context->internal_width / 2, scene->context->internal_height / 2))
			);

	entity->velocity.x = 0;
	entity->velocity.y = 0;

	if(keys[SDL_SCANCODE_A]){
		entity->velocity.x += -100;
	}
	if(keys[SDL_SCANCODE_D]){
		entity->velocity.x += 100;
	}
	if(keys[SDL_SCANCODE_W]){
		entity->velocity.y += -100;
	}
	if(keys[SDL_SCANCODE_S]){
		entity->velocity.y += 100;
	}
	
	entity->velocity = Vec2_Mul(Vec2_Normalize(entity->velocity), 100);

	//entity->next_think += 1;
}

Entity * Testent2_Create(Scene *scene){
	Entity *entity;

	entity = Entity_Create(scene); 
	Entity_SetClassname(entity, "classname2");

	entity->texture = Scene_GetTexture(scene, "player.png");
	entity->position = Vec2_Create(96, 96);

	entity->texture_id = 0;
	entity->hitbox_size = Vec2_Create(32, 32);
	entity->collision_layer = LAYER_MASK(2);
	entity->collision_mask = LAYER_MASK(1);

	entity->think = Testent2_Think;

	return entity;
}

void Testent2_Think(Scene *scene, Entity *entity){
}
