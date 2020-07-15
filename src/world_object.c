#define _USE_MATH_DEFINES
#include "sprite.h"

#include "olc/olc.h"

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

int init_world_object() {
    world_global = malloc(sizeof(world_t));
    world_global->player.pos.x = 1;
    world_global->player.pos.y = 1;
    world_global->player.angle = M_PI_4;
    world_global->player.speed = 2.5;
    world_global->player.angle_of_vision = M_PI_4;
    world_global->player.angular_speed = 1.5;
    init_bullet_array(world_global, 5);
    world_global->textures.wall = malloc(sizeof(sprite_t));
    world_global->textures.bullet = malloc(sizeof(sprite_t));
    init_sprite(8, 8, world_global->textures.wall);
    init_sprite(8, 8, world_global->textures.bullet);
    set_sprite_color(0, 0, world_global->textures.wall, FG_GREEN);
    set_sprite_color(1, 0, world_global->textures.wall, FG_BLUE);
    set_sprite_color(2, 0, world_global->textures.wall, FG_YELLOW);
    set_sprite_color(3, 0, world_global->textures.wall, FG_GREY);
    set_sprite_color(4, 0, world_global->textures.wall, FG_CYAN);
    set_sprite_color(5, 0, world_global->textures.wall, FG_DARK_BLUE);
    set_sprite_color(6, 0, world_global->textures.wall, FG_DARK_RED);
    set_sprite_color(7, 0, world_global->textures.wall, FG_RED);
    set_sprite_color(0, 7, world_global->textures.wall, FG_RED);
    set_sprite_color(1, 7, world_global->textures.wall, FG_RED);
    set_sprite_color(2, 7, world_global->textures.wall, FG_RED);
    set_sprite_color(3, 7, world_global->textures.wall, FG_RED);
    set_sprite_color(4, 7, world_global->textures.wall, FG_RED);
    set_sprite_color(5, 7, world_global->textures.wall, FG_RED);
    set_sprite_color(6, 7, world_global->textures.wall, FG_RED);
    set_sprite_color(7, 7, world_global->textures.wall, FG_RED);
    return read_map_for_file();
}

void deinit_world_object() {
    for (int i = 0; i < world_global->map_height; i++) {
        free(world_global->map[i]);
    }
    free(world_global->map);
    deinit_sprite(world_global->textures.wall);
    deinit_sprite(world_global->textures.bullet);
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
