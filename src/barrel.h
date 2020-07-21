#ifndef BARREL_H_
#define BARREL_H_

void add_barrel(world_t* world, point_t pos, double damage, double expl_radius, double radius);
void blow_barrel(world_t* world, int index);

#endif
