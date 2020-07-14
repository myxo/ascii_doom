#ifndef ENEMY_H_
#define ENEMY_H_

#include "world_object.h"

void add_enemy(world_t* world);
void enemy_destruct(world_t* world, int index);
void enemy_movement(world_t* world, float time_elapsed);

#endif
