#ifndef ROCKET_H_
#define ROCKET_H_

void increase_arr_rockets_capacity(world_t* world);

void rocket_destruct(world_t* world, int index);

void rockets_movement(world_t* world, float time_elapsed);

void shoot_rocket(world_t* world, point_t pos, double angle, bullet_host_t host, double damage, double expl_radius);

void make_explosion(point_t pos, double std_damage, double radius);

#endif // !ROCKET_H_
