#ifndef TESTENT_H
#define TESTENT_H

#include "entity.h"

Entity * Testent_Create(Scene *scene);

void Testent_Think(Scene *scene, Entity *entity);

Entity * Testent2_Create(Scene *scene);

void Testent2_Think(Scene *scene, Entity *entity);

#endif
