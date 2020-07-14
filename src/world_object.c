#define _USE_MATH_DEFINES
#include "world_object.h"

#include <stdlib.h>

#include <math.h>

world_t* world_global = NULL;

void init_bullet_array(world_t* world, int capacity) {
    world_global->bullet_array.capacity = capacity;
    world_global->bullet_array.len = 0;
    world_global->bullet_array.array = malloc(world_global->bullet_array.capacity * sizeof(bullet_t));
}

void init_enemy_array(world_t* world, int capacity) {
    world_global->enemy_array.capacity = capacity;
    world_global->enemy_array.len = 0;
    world_global->enemy_array.array = malloc(world_global->enemy_array.capacity * sizeof(bullet_t));
}

void init_world_object() {
    world_global = malloc(sizeof(world_t));
    world_global->player.pos.x = 1;
    world_global->player.pos.y = 1;
    world_global->player.angle = 0;
    world_global->player.speed = 1.5;
    world_global->player.angle_of_vision = M_PI_4;
    world_global->player.angular_speed = 0.02;
    world_global->player.radius = 0.2;
    strcpy(world_global->map[0], "###############");
    strcpy(world_global->map[1], "# #######     #");
    strcpy(world_global->map[2], "#  ######     #");
    strcpy(world_global->map[3], "#  ########  ##");
    strcpy(world_global->map[4], "#    #####  ###");
    strcpy(world_global->map[5], "###  ####  ####");
    strcpy(world_global->map[6], "####  ##  #####");
    strcpy(world_global->map[7], "#####    ######");
    strcpy(world_global->map[8], "######  #######");
    strcpy(world_global->map[9], "#####  ########");
    strcpy(world_global->map[10], "####  #########");
    strcpy(world_global->map[11], "###  ##########");
    strcpy(world_global->map[12], "##  ###########");
    strcpy(world_global->map[13], "#  ######     #");
    strcpy(world_global->map[14], "#          ####");
    strcpy(world_global->map[15], "###############");
    init_bullet_array(world_global, 5);
    init_enemy_array(world_global, 5);
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

int is_bullet(double x, double y) {
    for (int i = 0; i < world_global->bullet_array.len; i++) {
        double ox_vec = x - world_global->bullet_array.array[i].pos.x;
        double oy_vec = y - world_global->bullet_array.array[i].pos.y;
        double r = world_global->bullet_array.array[i].radius;
        if (pow(ox_vec, 2) + pow(oy_vec, 2) <= pow(r, 2)){
            return 1;
        }
    }
    return 0;
}

int is_player(double x, double y) {
    double dx = x - world_global->player.pos.x;
    double dy = y - world_global->player.pos.y;
    double r = world_global->player.radius;
    if (pow(dx, 2) + pow(dy, 2) <= pow(r, 2)) {
        return 1;
    }
    return 0;
}

int is_enemy(double x, double y) {
    for (int i = 0; i < world_global->enemy_array.len; i++) {
        double ox_vec = x - world_global->enemy_array.array[i].pos.x;
        double oy_vec = y - world_global->enemy_array.array[i].pos.y;
        double r = world_global->enemy_array.array[i].radius;
        if (pow(ox_vec, 2) + pow(oy_vec, 2) <= pow(r, 2)) {
            return i + 1;
        }
    }
    return 0;
}
