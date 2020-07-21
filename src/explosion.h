#ifndef EXPLOSION_H_
#define EXPLOSION_H_

void make_explosion(world_t* world, point_t pos, double std_damage, double radius);

void update_life_time(world_t* world, float time_elapsed);

#endif // !EXPLOSION_H_
