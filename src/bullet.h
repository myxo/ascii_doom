#ifndef BULLET_H_
#define BULLET_H_

#include "world_object.h"

typedef enum bullet_host {
    kBulletPlayer,
    kBulletEnemy
} bullet_host_t;

void bullet_destruct(world_t* world, int index);
void bullets_movement(world_t* world, float time_elapsed);
void shoot_bullet(world_t* world, point_t pos, double angle, float time_elapsed, bullet_host_t host);

#endif
