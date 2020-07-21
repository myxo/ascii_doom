#define _USE_MATH_DEFINES

#include "olc/olc.h"

#include "world_object.h"
#include "weapon.h"
#include "sprite.h"
#include "config.h"
#include "barrel.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

world_t* world_global = NULL;

void init_explosion_array() {
    world_global->explosion_array.capacity = 5;
    world_global->explosion_array.len = 0;
    world_global->explosion_array.array = malloc(world_global->explosion_array.capacity * sizeof(explosion_t));
}

void deinit_explosion_array() {
    free(world_global->explosion_array.array);
}

void init_bullet_array(world_t* world, int capacity) {
    world_global->bullet_array.capacity = capacity;
    world_global->bullet_array.len = 0;
    world_global->bullet_array.array = malloc(world_global->bullet_array.capacity * sizeof(bullet_t));
}

void init_rocket_array(int capacity) {
    world_global->rocket_array.capacity = capacity;
    world_global->rocket_array.len = 0;
    world_global->rocket_array.array = malloc(world_global->rocket_array.capacity * sizeof(rocket_t));
}

void deinit_rocket_array() {
    free(world_global->rocket_array.array);
}

void init_enemy_array(world_t* world, int capacity) {
    world_global->enemy_array.capacity = capacity;
    world_global->enemy_array.len = 0;
    world_global->enemy_array.array = malloc(world_global->enemy_array.capacity * sizeof(enemy_t));
}

void init_barrel_array() {
    world_global->barrel_array.capacity = 5;
    world_global->barrel_array.len = 0;
    world_global->barrel_array.array = malloc(world_global->barrel_array.capacity * sizeof(barrel_t));
}

void deinit_barrel_array() {
    free(world_global->barrel_array.array);
}

void spawn_barrels() {
    while (world_global->barrel_array.len < 5) {
        int new_x = rand() % world_global->map_height;
        int new_y = rand() % world_global->map_width;
        double x_float = (rand() % 10) / 10;
        double y_float = (rand() % 10) / 10;
        if (!is_wall(new_x + x_float, new_y + y_float)) {
            point_t point = {new_x + x_float, new_y + y_float};
            add_barrel(world_global, point, 2, 3, 0.1);
        }
    }
}

point_array_t init_point_array(int capacity) {
    point_array_t array;
    array.capacity = capacity;
    array.len = 0;
    array.array = malloc(array.capacity * sizeof(point_t));
    return array;
}

void increase_arr_point_capacity(point_array_t* point_array) {
    point_array->capacity = point_array->capacity * 2;
    point_array->array = realloc(point_array->array, point_array->capacity * sizeof(point_t));
}
void init_sprites(world_t* world) {
    world_global->sprites.wall = malloc(sizeof(sprite_t));
    world_global->sprites.bullet = malloc(sizeof(sprite_t));
    world_global->sprites.mob1 = malloc(sizeof(sprite_t));
    init_sprite(world_global->sprites.wall);
    init_sprite(world_global->sprites.bullet);
    init_sprite(world_global->sprites.mob1);
    load_texture_from_file("wall1.tex", &world->textures.wall);
    attach_texture_to_sprite(world->sprites.wall, world->textures.wall);
    load_texture_from_file("mob1.tex", &world->textures.mob1);
    attach_texture_to_sprite(world->sprites.mob1, world->textures.mob1);
    load_texture_from_file("bullet1.tex", &world->textures.bullet);
    attach_texture_to_sprite(world->sprites.bullet, world->textures.bullet);
}

int init_world_object() {
    world_global = malloc(sizeof(world_t));
    update_world_from_config();

    world_global->player.health = 3;
    world_global->player.maxhealth = 3;
    world_global->player.angle = M_PI_4;
    world_global->player.radius = 0.2;

    world_global->weapon_list = malloc(sizeof(std_weapon_list_t));
    init_std_weapon_list(world_global->weapon_list);
    init_z_buffer();
    init_sprites(world_global);

    init_bullet_array(world_global, 5);
    init_enemy_array(world_global, 5);
    init_rocket_array(5);
    init_explosion_array();
    init_barrel_array();
    return read_map_for_file();
}

void deinit_world_object() {
    for (int i = 0; i < world_global->map_height; i++) {
        free(world_global->map[i]);
    }
    free(world_global->map);
    deinit_explosion_array();
    deinit_std_weapon_list(world_global->weapon_list);
    deinit_sprite(world_global->sprites.wall);
    deinit_rocket_array(5);
    deinit_barrel_array();
    deinit_texture(&world_global->textures.wall);
    deinit_z_buffer();
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
    for (int i = 0; i < olc_screen_width(); i++) {
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

int is_barrel(point_t pos, int* index){
    for (int i = 0; i < world_global->barrel_array.len; i++) {
        if (is_in_circle(pos, world_global->barrel_array.array[i].pos, world_global->barrel_array.array[i].radius)) {
            *index = i;
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
