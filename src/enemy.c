#define _USE_MATH_DEFINES

#include "sprite.h"
#include "world_object.h"
#include "bullet.h"
#include "../third_party/olc/olc.h"

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "structs_of_data.h"

void increase_arr_enemy_capacity(world_t* world) {
    world->enemy_array.capacity = world->enemy_array.capacity * 2;
    world->enemy_array.array = realloc(world->enemy_array.array, world->enemy_array.capacity * sizeof(enemy_t));
}

point_t get_new_forward_pos(point_t pos, double angle, double time_elapsed, double speed) {
    pos.x += time_elapsed * speed * sin(angle);
    pos.y += time_elapsed * speed * cos(angle);
    return pos;
}

point_array_t build_path(enemy_t* enemy) {
    point_queue_t q = point_queue_init();
    point_queue_push_back(&q, enemy->pos);
    int** used = malloc(get_world()->map_width * sizeof(int*));
    for (int i = 0; i < get_world()->map_width; i++) {
        used[i] = malloc(get_world()->map_height * sizeof(int));
        for (int j = 0; j < get_world()->map_height; j++) {
            used[i][j] = 0;
        }
    }
    used[(int)enemy->pos.x][(int)enemy->pos.y] = 1;
    point_t** pred = malloc(get_world()->map_width * sizeof(point_t*));
    for (int i = 0; i < get_world()->map_width; i++) {
        pred[i] = malloc(get_world()->map_height * sizeof(point_t));
    }
    point_t null = { -1, -1 };
    pred[(int)enemy->pos.x][(int)enemy->pos.y] = null;
    while (!isempty_point_queue(q)) {
        point_t cur = point_queue_pop(&q);
        point_array_t point_near = init_point_array(8);
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                //if (x == 0 && y == 0)
                //    continue;
                if (abs(x) == abs(y))
                    continue;
                if (!is_wall(cur.x + x, cur.y + y)) {
                    point_t temp = { cur.x + x, cur.y + y };
                    point_near.array[point_near.len++] = temp;
                }
            }
        }
        for (int i = 0; i < point_near.len; ++i) {
            point_t to = point_near.array[i];
            if (!used[(int)to.x][(int)to.y]) {
                used[(int)to.x][(int)to.y] = 1;
                point_queue_push_back(&q, to);
                pred[(int)to.x][(int)to.y] = cur;
                //if (to.x == enemy->global_target.x && to.y == enemy->global_target.y)
                //    i++;
            }
        }
    }
    point_array_t reverse_path = init_point_array(8);
    for (point_t cur = enemy->global_target; !(cur.x == -1 && cur.y == -1); cur = pred[(int)cur.x][(int)cur.y]) {
        if (reverse_path.len >= reverse_path.capacity)
            increase_arr_point_capacity(&reverse_path);
        reverse_path.array[reverse_path.len++] = cur;
    }
    point_array_t path = init_point_array(reverse_path.len);
    for (int i = 0; i < reverse_path.len; i++) {
        point_t temp = reverse_path.array[reverse_path.len - 1 - i];
        //temp.x = temp.x + 0.5;
        //temp.y = temp.y + 0.5;
        path.array[i] = temp;
        path.len++;
    }
    return path;
}

void add_enemy(world_t* world) {
    if (world->enemy_array.len >= world->enemy_array.capacity - 1)
        increase_arr_enemy_capacity(world);
    world->enemy_array.array[world->enemy_array.len].health = 3;
    world->enemy_array.array[world->enemy_array.len].pos = get_rand_pos_on_floor(world);
    world->enemy_array.array[world->enemy_array.len].global_target = get_rand_pos_on_floor(world);
    world->enemy_array.array[world->enemy_array.len].path = build_path(&world->enemy_array.array[world->enemy_array.len]);
    world->enemy_array.array[world->enemy_array.len].local_target_id = 0;
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
        if (is_in_circle(enemy->global_target, enemy->pos, 0.5)) {
            enemy->global_target = get_rand_pos_on_floor(world);
            enemy->path = build_path(enemy);
            enemy->local_target_id = 0;
        }
        enemy->angle = get_angle_from_pos1_to_pos2(enemy->pos, enemy->path.array[enemy->local_target_id]);

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


                    shoot_bullet(world, enemy->pos, angle_to_player, time_elapsed, kBulletEnemy);
                }
                if (distance_to_player <= 4) {
                    update_position = 0;
                }
                enemy->global_target = world->player.pos;
                enemy->path = build_path(enemy);
            }
        }
        if (update_position) {
            point_t new_pos = get_new_forward_pos(enemy->pos, enemy->angle, time_elapsed, enemy->speed);
            if (!is_wall(new_pos.x, new_pos.y) && !is_in_circle(enemy->path.array[enemy->local_target_id], enemy->pos, enemy->radius)) {
                enemy->pos = new_pos;
            }
            if (!is_in_circle(enemy->path.array[enemy->local_target_id], enemy->pos, enemy->radius)) {
                enemy->local_target_id++;
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
