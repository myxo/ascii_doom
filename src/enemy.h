#ifndef ENEMY_H_
#define ENEMY_H_

#include "world_object.h"

void bullet_destruct(world_t* world, int index);
void bullets_movement(world_t* world, float time_elapsed);
void shoot_bullet(world_t* world, float time_elapsed);

#endif
