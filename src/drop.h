#ifndef DROP_H_
#define DROP_H_

#include "world_object.h"

enum types_of_crate {
    FIRST_AID = 0,
    BULLETS = 1,
};

void add_drop(world_t* world, point_t pos);
void drop_check(world_t* world);
void init_drop_spawner(world_t* world);
void drop_timer(world_t* world, double time_elapsed);

#endif
