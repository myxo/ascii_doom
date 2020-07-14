#define _USE_MATH_DEFINES
#include "world_object.h"

#include <stdlib.h>

#include <stdio.h>

#include <math.h>


world_t * world_global = NULL;

int init_world_object() {
    world_global = malloc(sizeof(world_t));
    world_global->player.pos.x = 1;
    world_global->player.pos.y = 1;
    world_global->player.angle = 0;
    world_global->player.speed = 1.5;
    world_global->player.angle_of_vision = M_PI_4;
    world_global->player.angular_speed = 0.02;
    return read_map_for_file();
}

void deinit_world_object() {
    free(world_global);
}

world_t* get_world() {
    return world_global;
}

int is_wall(double x, double y) {
    return world_global->map[(int)x][(int)y] == '#';
}

int read_map_for_file() {
    FILE* fmap;
    fmap = fopen("map.txt", "r");
    if (fmap == NULL) {
        return 0;
    }
    char tmp;
    fscanf(fmap, "%d %d", &world_global->map_width, &world_global->map_height);
    world_global->map = malloc(world_global->map_height * sizeof(char*));
    for (int i = 0; i < world_global->map_height; i++) {
        world_global->map[i] = malloc((world_global->map_width + 1) * sizeof(char));
    }
    fgetc(fmap);
    for (int i = 0; i < world_global->map_height; i++) {
        for (int j = 0;  j < world_global->map_width; j++) {
            world_global->map[i][j] = fgetc(fmap);
        }
        world_global->map[i][world_global->map_width] = 0;
        fgetc(fmap);
    }
    fclose(fmap);
    return 1;
}
