#define _USE_MATH_DEFINES
#include "world_object.h"

#include <stdlib.h>

#include <stdio.h>

#include <math.h>


world_t * world_global = NULL;

void init_world_object() {
    world_global = malloc(sizeof(world_t));
    world_global->player.pos.x = 1;
    world_global->player.pos.y = 1;
    world_global->player.angle = 0;
    world_global->player.speed = 1.5;
    world_global->player.angle_of_vision = M_PI_4;
    world_global->player.angular_speed = 0.02;
    read_map_for_file();
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

void read_map_for_file() {
    FILE* fmap;
    fmap = fopen("map.txt", "r");
    char tmp;
    int i = 0;
    int length = 0;
    int capacity = 1;
    int vertical_capacity = 1;
    world_global->map = malloc(sizeof(char*));
    world_global->map[0] = malloc(sizeof(char));
    world_global->map_height = 0;
    tmp = fgetc(fmap);
    while (tmp != EOF) {
        while (tmp != '\n') {
            world_global->map[i][length++] = tmp;
            if (length == capacity) {
                capacity *= 2;
                world_global->map[i] = realloc(world_global->map[i], (capacity + 1) * sizeof(char));
            }
            tmp = fgetc(fmap);
        }
        if (length > world_global->map_height) {
            world_global->map_height = length;
        }
        world_global->map[i][length] = 0;
        world_global->map = realloc(world_global->map, (++vertical_capacity)* sizeof(char*));
        i++;
        length = 0;
        capacity = 1;
        world_global->map[i] = malloc(sizeof(char));
        tmp = fgetc(fmap);
    }
    world_global->map_height = i;
    fclose(fmap);
}
