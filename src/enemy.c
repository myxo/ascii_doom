#define _USE_MATH_DEFINES

#include "sprite.h"
#include "world_object.h"
#include "bullet.h"

#include <stdlib.h>
#include <math.h>
#include <time.h>

void increase_arr_enemy_capacity(world_t* world) {
    world->enemy_array.capacity = world->enemy_array.capacity * 2;
    world->enemy_array.array = realloc(world->enemy_array.array, world->enemy_array.capacity * sizeof(enemy_t));
}

point_t get_new_forward_pos(point_t pos, double angle, double time_elapsed, double speed) {
    pos.x += time_elapsed * speed * sin(angle);
    pos.y += time_elapsed * speed * cos(angle);
    return pos;
}

void add_enemy(world_t* world) {
    if (world->enemy_array.len >= world->enemy_array.capacity - 1)
        increase_arr_enemy_capacity(world);
    world->enemy_array.array[world->enemy_array.len].health = 3;
    point_t pos = world->enemy_array.array[world->enemy_array.len].pos;
    point_t target = world->enemy_array.array[world->enemy_array.len].target;
    do {
        target = get_rand_pos_on_floor(world);
    } while (is_wall(target.x, target.y));
    do {
        pos = get_rand_pos_on_floor(world);
    } while (is_wall(pos.x, pos.y));
    world->enemy_array.array[world->enemy_array.len].target = target;
    world->enemy_array.array[world->enemy_array.len].pos = pos;
    world->enemy_array.array[world->enemy_array.len].angle = 0;
    world->enemy_array.array[world->enemy_array.len].speed = 1.5;
    world->enemy_array.array[world->enemy_array.len].angle_of_vision = M_PI_2;
    world->enemy_array.array[world->enemy_array.len].radius = 0.2;
    world->enemy_array.array[world->enemy_array.len].time_from_last_shot = 0;
    world->enemy_array.len++;
}         

void enemy_movement(world_t* world, float time_elapsed) {
    for (int i = 0; i < world->enemy_array.len; i++) {
        enemy_t* enemy = &world->enemy_array.array[i];
        enemy->angle = get_angle_from_pos1_to_pos2(enemy->pos, enemy->target);

        double angle_to_player = get_angle_from_pos1_to_pos2(enemy->pos, world->player.pos);
        double start_enemy_view_angle = enemy->angle - enemy->angle_of_vision / 2;
        double stop_enemy_view_angle = enemy->angle + enemy->angle_of_vision / 2;
        double x = enemy->pos.x;
        double y = enemy->pos.y;
        double d_distance = 0.01;
        int update_position = 1;
        enemy->time_from_last_shot += time_elapsed;
        if (angle_to_player > start_enemy_view_angle && angle_to_player < stop_enemy_view_angle) {
            double distance_to_player = get_distance_from_pos1_to_pos2(enemy->pos, world->player.pos);
            if (!has_wall_between(enemy->pos, world->player.pos)) {
                if (distance_to_player <= 10 && enemy->time_from_last_shot >= 2) {
                    enemy->time_from_last_shot = 0;
                    shoot_bullet(world, enemy->pos, angle_to_player, time_elapsed, kBulletEnemy, 1);
                }
                if (distance_to_player <= 4) {
                    update_position = 0;
                }
                enemy->target = world->player.pos;
            }
        }
        if (update_position) {
            point_t new_pos = get_new_forward_pos(enemy->pos, enemy->angle, time_elapsed, enemy->speed);
            if (!is_wall(new_pos.x, new_pos.y) && !is_enemy(enemy->target.x, enemy->target.y, NULL)) {
                enemy->pos = new_pos;
            }
            else {
                do {
                    enemy->target = get_rand_pos_on_floor(world);
                } while (is_wall(enemy->target.x, enemy->target.y));
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
