#define _USE_MATH_DEFINES

#include "sprite.h"
#include "world_object.h"
#include "bullet.h"
#include "olc/olc.h"
#include "enemy.h"
#include "player.h"

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
    point_t temp;
    temp.x = (int)enemy->pos.x;
    temp.y = (int)enemy->pos.y;
    point_queue_t q = point_queue_init();
    point_queue_push_back(&q, temp);
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
    int global_target_found = 0;
    while (!isempty_point_queue(q)) {
        point_t cur = point_queue_pop(&q);
        int x_move[4] = { -1, 0,  0, 1 };
        int y_move[4] = { 0, 1, -1, 0 };
        for (int i = 0; i < 4; i++) {
            int x = (int)cur.x + x_move[i];
            int y = (int)cur.y + y_move[i];
            if (!is_wall(x, y)) {
                point_t to = { x, y };
                if (to.x > 0 && to.y > 0 && to.x < get_world()->map_width && to.y < get_world()->map_height && !used[x][y]) {
                    used[x][y] = 1;
                    point_queue_push_back(&q, to);
                    pred[x][y] = cur;
                    if (x == (int)enemy->global_target.x && y == (int)enemy->global_target.y) {
                        global_target_found = 1;
                        break;
                    }
                }
            }
        }
        if (global_target_found)
            break;
    }
    point_array_t path = init_point_array(8);
    point_t temp_point = { enemy->pos.x, enemy->pos.y };
    path.array[path.len++] = temp_point;
    if (!global_target_found)
        return path;
    point_array_t reverse_path = init_point_array(8);
    for (point_t cur = enemy->global_target; !(cur.x == -1 && cur.y == -1); cur = pred[(int)cur.x][(int)cur.y]) {
        if (reverse_path.len >= reverse_path.capacity)
            increase_arr_point_capacity(&reverse_path);
        reverse_path.array[reverse_path.len++] = cur;
    }
    for (int i = 0; i < reverse_path.len; i++) {
        if (path.len >= path.capacity)
            increase_arr_point_capacity(&path);
        point_t temp = reverse_path.array[reverse_path.len - 1 - i];
        temp.x = temp.x + 0.5;
        temp.y = temp.y + 0.5;
        path.array[i] = temp;
        path.len++;
    }
    free(reverse_path.array);
    for (int i = 0; i < get_world()->map_width; i++) {
        free(pred[i]);
    }
    free(pred);
    for (int i = 0; i < get_world()->map_width; i++) {
        free(used[i]);
    }
    free(used);
    return path;
}

void add_shooter_enemy(world_t* world) {
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
    world->enemy_array.array[world->enemy_array.len].last_player_pos = world->player.pos;
    world->enemy_array.array[world->enemy_array.len].type = shooter;
    world->enemy_array.len++;
}

void add_hound_enemy(world_t* world) {
    if (world->enemy_array.len >= world->enemy_array.capacity - 1)
        increase_arr_enemy_capacity(world);
    world->enemy_array.array[world->enemy_array.len].health = 3;
    world->enemy_array.array[world->enemy_array.len].pos = get_rand_pos_on_floor(world);
    world->enemy_array.array[world->enemy_array.len].global_target = world->player.pos;
    world->enemy_array.array[world->enemy_array.len].path = build_path(&world->enemy_array.array[world->enemy_array.len]);
    world->enemy_array.array[world->enemy_array.len].local_target_id = 0;
    world->enemy_array.array[world->enemy_array.len].angle = 0;
    world->enemy_array.array[world->enemy_array.len].speed = 1.5;
    world->enemy_array.array[world->enemy_array.len].angle_of_vision = M_PI_2;
    world->enemy_array.array[world->enemy_array.len].radius = 0.2;
    world->enemy_array.array[world->enemy_array.len].time_from_last_shot = 0;
    world->enemy_array.array[world->enemy_array.len].last_player_pos = world->player.pos;
    world->enemy_array.array[world->enemy_array.len].type = hound;
    world->enemy_array.len++;
}

void add_enemy(world_t* world, type_of_enemy_t type) {
    switch (type)
    {
    case hound:
        add_hound_enemy(world);
        break;
    case shooter:
        add_shooter_enemy(world);
        break;
    }
}

void bite_player(world_t* world, double damage) {
    player_hit(1);
}

void enemy_movement(world_t* world, float time_elapsed) {
    for (int i = 0; i < world->enemy_array.len; i++) {
        enemy_t* enemy = &world->enemy_array.array[i];
        int update_position = 1;
        if (is_in_circle(enemy->global_target, enemy->pos, 0.5)) {
            if (enemy->type == shooter) {
                enemy->global_target = get_rand_pos_on_floor(world);
                enemy->path = build_path(enemy);
                enemy->local_target_id = 0;
            }
            else if (enemy->type == hound) {
                if (enemy->time_from_last_shot >= 2) {
                    enemy->time_from_last_shot = 0;
                    bite_player(world, 1);
                }
                update_position = 0;
            }
        }
        enemy->angle = get_angle_from_pos1_to_pos2(enemy->pos, enemy->path.array[enemy->local_target_id]);

        double angle_to_player = get_angle_from_pos1_to_pos2(enemy->pos, world->player.pos);
        double start_enemy_view_angle = enemy->angle - enemy->angle_of_vision / 2;
        double stop_enemy_view_angle = enemy->angle + enemy->angle_of_vision / 2;
        double x = enemy->pos.x;
        double y = enemy->pos.y;
        double d_distance = 0.01;
        enemy->time_from_last_shot += time_elapsed;
        if (enemy->type == shooter) {
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
                    double delta = sqrt(pow(world->player.pos.x - enemy->last_player_pos.x, 2) + pow(world->player.pos.y - enemy->last_player_pos.y, 2));
                    if (delta >= 0.5) {
                        enemy->global_target = world->player.pos;
                        enemy->path = build_path(enemy);
                        enemy->local_target_id = 0;
                        enemy->last_player_pos = world->player.pos;
                    }
                }
            }
        }
        else if (enemy->type == hound && is_in_circle(enemy->global_target, enemy->pos, 0.5)) {
            double delta = sqrt(pow(world->player.pos.x - enemy->last_player_pos.x, 2) + pow(world->player.pos.y - enemy->last_player_pos.y, 2));
            if (delta >= 0.5) {
                enemy->global_target = world->player.pos;
                enemy->path = build_path(enemy);
                enemy->local_target_id = 0;
                enemy->last_player_pos = world->player.pos;
                update_position = 1;
            }
        }
        if (update_position) {
            point_t new_pos = get_new_forward_pos(enemy->pos, enemy->angle, time_elapsed, enemy->speed);
            if (!is_wall(new_pos.x, new_pos.y) && !is_in_circle(enemy->path.array[enemy->local_target_id], enemy->pos, enemy->radius)) {
                enemy->pos = new_pos;
            }
            if (is_in_circle(enemy->path.array[enemy->local_target_id], enemy->pos, enemy->radius)) {
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

void enemy_hit(world_t* world, int index, double damage) {
    world->enemy_array.array[index].health -= damage;
    if (world->enemy_array.array[index].health <= 0)
        enemy_destruct(world, index);
}
