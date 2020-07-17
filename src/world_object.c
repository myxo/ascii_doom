#define _USE_MATH_DEFINES

#include "olc/olc.h"

#include "world_object.h"
#include "weapon.h"
#include "sprite.h"
#include "config.h"

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
    update_world_from_config();

    world_global->player.health = 3;
    world_global->player.pos.x = 1;
    world_global->player.pos.y = 1;
    world_global->player.angle = M_PI_4;
    world_global->player.radius = 0.2;

    world_global->textures.wall = malloc(sizeof(sprite_t));
    world_global->textures.bullet = malloc(sizeof(sprite_t)); 
    world_global->weapon_list = malloc(sizeof(std_weapon_list_t));
    init_std_weapon_list(world_global->weapon_list);
    init_z_buffer();
    init_sprite(8, 8, world_global->textures.wall);
    init_sprite(8, 8, world_global->textures.bullet);
    load_sprite_from_file("wall1.spr", world_global->textures.wall);

    init_bullet_array(world_global, 5);
    init_enemy_array(world_global, 5);
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

void init_z_buffer() {
    world_global->z_buffer = calloc(olc_screen_width(), (olc_screen_width() + 1) * sizeof(double*));
    for (int i = 0; i <= olc_screen_width(); i++) {
        world_global->z_buffer[i] = calloc(olc_screen_height(), (olc_screen_height() + 1) * sizeof(double));
    }
    for (int i = 0; i < olc_screen_width(); i++) {
        for (int j = 0; j < olc_screen_height(); j++) {
            world_global->z_buffer[i][j] = MAX_BUFF;
        }
    }
}

void deinit_z_buffer() {
    for (int i = 0; i < olc_screen_width; i++) {
        free(world_global->z_buffer[i]);
    }
    free(world_global->z_buffer);
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
    return pow(dx, 2) + pow(dy, 2) <= pow(radius, 2);
}

int is_player(double x, double y) {
    point_t pos = {x , y};
    return is_in_circle(pos, world_global->player.pos, world_global->player.radius);
}

int is_enemy(double x, double y, int* enemy_index) {
    for (int i = 0; i < world_global->enemy_array.len; i++) {
        point_t pos = {x, y};
        if (is_in_circle(pos, world_global->enemy_array.array[i].pos, world_global->enemy_array.array[i].radius)) {
            if (enemy_index != NULL)
                *enemy_index = i;
            return 1;
        }
    }
    return 0;
}

point_t get_rand_pos_on_floor(world_t* world) {
    point_t pos;
    do {
        pos.x = rand() % world->map_width;
        pos.y = rand() % world->map_height;
    } while (is_wall(pos.x, pos.y));
    return pos;
}

double get_angle_from_pos1_to_pos2(point_t pos1, point_t pos2) {
    double delta_x = pos2.x - pos1.x;
    double delta_y = pos2.y - pos1.y;
    return atan2(delta_x, delta_y);
}

double get_distance_from_pos1_to_pos2(point_t pos1, point_t pos2) {
    return sqrt(pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2));
}

int has_wall_between_by_angle(point_t pos1, point_t pos2, double angle, double d_distance) {
    double x = pos1.x;
    double y = pos1.y;
    while (!is_wall(x, y)) {
        x += d_distance * sin(angle);
        y += d_distance * cos(angle);
        point_t pos = { x, y };
        if (is_in_circle(pos, pos2, 0.1)) {
            return 0;
        }
    }
    return 1;
}


int has_wall_between(point_t pos1, point_t pos2) {
    return has_wall_between_by_angle(pos1, pos2, get_angle_from_pos1_to_pos2(pos1, pos2), 0.1);
}

void update_world_from_config() {
    world_global->player.speed = get_config_value(kPlayerSpeed);
    world_global->player.angle_of_vision = get_config_value(kAngleOfView);
    world_global->player.angular_speed = get_config_value(kPlayerAngularSpeed);
}
