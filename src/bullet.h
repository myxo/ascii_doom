#ifndef BULLET_H_
#define BULLET_H_

void bullet_destruct(world_t* world, int index);
void bullets_movement(world_t* world, float time_elapsed);
void increase_arr_bullets_capacity(world_t* world);
void shoot_bullet(world_t* world, point_t pos, double angle, float time_elapsed, bullet_host_t host, double damage);

#endif
