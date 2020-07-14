#define _USE_MATH_DEFINES

#include "world_object.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>


void increase_arr_enemy_capacity(world_t* world) {
    world->enemy_array.capacity = world->enemy_array.capacity * 2;
    world->enemy_array.array = realloc(world->enemy_array.array, world->enemy_array.capacity * sizeof(bullet_t));
}

void add_enemy(world_t* world) {
    if (world->enemy_array.len >= world->enemy_array.capacity - 1)
        increase_arr_enemy_capacity(world);
    world->enemy_array.array[world->enemy_array.len].health = 3;
    do {
        world->enemy_array.array[world->enemy_array.len].target.x = rand() % 16;
        world->enemy_array.array[world->enemy_array.len].target.y = rand() % 16;
    } while (is_wall(world->enemy_array.array[world->enemy_array.len].target.x, world->enemy_array.array[world->enemy_array.len].target.y));
    do {
        world->enemy_array.array[world->enemy_array.len].pos.x = rand() % 16;
        world->enemy_array.array[world->enemy_array.len].pos.y = rand() % 16;
    } while (is_wall(world->enemy_array.array[world->enemy_array.len].pos.x, world->enemy_array.array[world->enemy_array.len].pos.y));
    world->enemy_array.array[world->enemy_array.len].angle = 0;
    world->enemy_array.array[world->enemy_array.len].speed = 1.5;
    world->enemy_array.array[world->enemy_array.len].angle_of_vision = M_PI_2;
    world->enemy_array.array[world->enemy_array.len].radius = 0.2;
    world->enemy_array.len++;
}

double get_angle_from_pos_to_angle(point_t pos, point_t target) {
    double delta_x = target.x - pos.x;
    double delta_y = target.y - pos.y;
    double x = atan2(delta_x, delta_y);
    return x;
}                

void enemy_movement(world_t* world, float time_elapsed) {
    for (int i = 0; i < world->enemy_array.len; i++) {
        world->enemy_array.array[i].angle = get_angle_from_pos_to_angle(world->enemy_array.array[i].pos, world->enemy_array.array[i].target);

        double angle_to_player = get_angle_from_pos_to_angle(world->enemy_array.array[i].pos, world->player.pos);
        double start_enemy_view_angle = world->enemy_array.array[i].angle - world->enemy_array.array[i].angle_of_vision / 2;
        double stop_enemy_view_angle = world->enemy_array.array[i].angle + world->enemy_array.array[i].angle_of_vision / 2;
        double x = world->enemy_array.array[i].pos.x;
        double y = world->enemy_array.array[i].pos.y;
        double distance_to_player = 0;
        double d_distance = 0.01;
        int check_move = 1;
        if (angle_to_player > start_enemy_view_angle && angle_to_player < stop_enemy_view_angle) {
            while (!is_wall(x, y)) {
                x += d_distance * sin(angle_to_player);
                y += d_distance * cos(angle_to_player);
                distance_to_player += d_distance;
                if (is_player(x, y)) {
                    if (distance_to_player <= 4) {
                        check_move = 0;
                    }
                    world->enemy_array.array[i].target = world->player.pos;
                    break;
                }
            }
        }
        if (check_move) {
            double new_x = world->enemy_array.array[i].pos.x;
            new_x += time_elapsed * world->enemy_array.array[i].speed * sin(world->enemy_array.array[i].angle);
            double new_y = world->enemy_array.array[i].pos.y;
            new_y += time_elapsed * world->enemy_array.array[i].speed * cos(world->enemy_array.array[i].angle);
            if (!is_wall(new_x, new_y) && !is_enemy(world->enemy_array.array[i].target.x, world->enemy_array.array[i].target.y)) {
                world->enemy_array.array[i].pos.x = new_x;
                world->enemy_array.array[i].pos.y = new_y;
            }
            else {
                do {
                    world->enemy_array.array[i].target.x = rand() % 16;
                    world->enemy_array.array[i].target.y = rand() % 16;
                } while (is_wall(world->enemy_array.array[i].target.x, world->enemy_array.array[i].target.y));
            }
        }
    }
}

void enemy_destruct(world_t* world, int index) {
    for (int i = index; i < world->enemy_array.len - 1; i++) {
        world->enemy_array.array[i] = world->enemy_array.array[i + 1];
    }
    world->enemy_array.len--;
}

void enemy_hit(world_t* world, int index, int damage) {
    world->enemy_array.array[index].health -= damage;
    if (world->enemy_array.array[index].health <= 0)
        enemy_destruct(world, index);
}
