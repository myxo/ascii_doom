#define _USE_MATH_DEFINES
#include "world_object.h"

#include <stdlib.h>

#include <stdio.h>

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
    world_global->enemy_array.array = malloc(world_global->enemy_array.capacity * sizeof(enemy_t));
}


int init_world_object() {
    world_global = malloc(sizeof(world_t));
    world_global->player.health = 3;
    world_global->player.pos.x = 1;
    world_global->player.pos.y = 1;
    world_global->player.angle = 0;
    world_global->player.speed = 1.5;
    world_global->player.angle_of_vision = M_PI_4;
    world_global->player.angular_speed = 0.02;
    world_global->player.radius = 0.2;
    init_bullet_array(world_global, 5);
    init_enemy_array(world_global, 5);
    return read_map_for_file();
}

void deinit_world_object() {
    for (int i = 0; i < world_global->map_height; i++) {
        free(world_global->map[i]);
    }
    free(world_global->map);
    free(world_global);
}

world_t* get_world() {
    return world_global;
}

int is_wall(double x, double y) {
    if (x < 0 || y < 0)
        return 1;
    if ((int)x >= get_world()->map_height || (int)y >= get_world()->map_width)
        return 1;
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
        for (int j = 0; j < world_global->map_width; j++) {
            world_global->map[i][j] = fgetc(fmap);
        }
        world_global->map[i][world_global->map_width] = 0;
        fgetc(fmap);
    }
    fclose(fmap);
    return 1;
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

int is_in_circle(point_t pos, point_t circle_center, double radius) {
    double dx = pos.x - circle_center.x;
    double dy = pos.y - circle_center.y;
    if (pow(dx, 2) + pow(dy, 2) <= pow(radius, 2)) {
        return 1;
    }
    return 0;
}

int is_player(double x, double y) {
    point_t pos;
    pos.x = x;
    pos.y = y;
    return is_in_circle(pos, world_global->player.pos, world_global->player.radius);
}

int is_enemy(double x, double y, int* enemy_index) {
    for (int i = 0; i < world_global->enemy_array.len; i++) {
        point_t pos;
        pos.x = x;
        pos.y = y;
        if (is_in_circle(pos, world_global->enemy_array.array[i].pos, world_global->enemy_array.array[i].radius)) {
            *enemy_index = i;
            return 1;
        }
    }
    return 0;
}

point_t get_rand_pos_on_floor(world_t* world) {
    point_t pos;
    pos.x = rand() % world->map_width;
    pos.y = rand() % world->map_height;
    return pos;
}

