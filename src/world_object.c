#include "world_object.h"

#include <stdlib.h>

world_t * world_global = NULL;

void init_world_object() {
    world_global = malloc(sizeof(world_t));
}

void deinit_world_object() {
    free(world_global);
}

world_t* get_world() {
    return world_global;
}
