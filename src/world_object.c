#define _USE_MATH_DEFINES
#include "world_object.h"

#include <stdlib.h>

#include <math.h>

world_t * world_global = NULL;

void init_world_object() {
    world_global = malloc(sizeof(world_t));
    world_global->player.pos.x = 2;
    world_global->player.pos.y = 6;
    world_global->player.angle = 0;
    world_global->player.speed = 1.5;
    world_global->player.angle_of_vision = M_PI_4;
    world_global->player.angular_speed = 0.02;
}

void deinit_world_object() {
    free(world_global);
}

world_t* get_world() {
    return world_global;
}
