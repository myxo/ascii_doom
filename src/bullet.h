#ifndef BULLET_H_
#define BULLET_H_

#include "world_object.h"

#define PLAYER_BULLET 0
#define ENEMY_BULLET 1

void bullet_destruct(world_t* world, int index);
void bullets_movement(world_t* world, float time_elapsed);
void shoot_bullet(world_t* world, point_t pos, double angle, float time_elapsed, int host);

#endif
