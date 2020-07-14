#define _USE_MATH_DEFINES
#include "world_object.h"

#include <stdlib.h>

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
    strcpy(world_global->map[0],  "################");
    strcpy(world_global->map[1],  "# #######     ##");
    strcpy(world_global->map[2],  "#  ######     ##");
    strcpy(world_global->map[3],  "#  ########  ###");
    strcpy(world_global->map[4],  "#    #####  ####");
    strcpy(world_global->map[5],  "###  ####  #####");
    strcpy(world_global->map[6],  "####  ##  ######");
    strcpy(world_global->map[7],  "#####    #######");
    strcpy(world_global->map[8],  "######  ########");
    strcpy(world_global->map[9],  "#####  #########");
    strcpy(world_global->map[10], "####  ##########");
    strcpy(world_global->map[11], "###  ###########");
    strcpy(world_global->map[12], "##  ############");
    strcpy(world_global->map[13], "#  ######     ##");
    strcpy(world_global->map[14], "#          #####");
    strcpy(world_global->map[15], "################");
    world_global->map_width = 16;
    world_global->map_height = 16;
    world_global->seeable = calloc(world_global->map_height, world_global->map_height * sizeof(int));
    for (int i = 0; i < world_global->map_height; i++) {
        world_global->seeable[i] = calloc(world_global->map_width, world_global->map_width * sizeof(int));
    }
}

void deinit_world_object() {
    free(world_global->seeable);
    free(world_global);
}

world_t* get_world() {
    return world_global;
}

int is_wall(double x, double y) {
    return world_global->map[(int)x][(int)y] == '#';
}
