#include "sprite.h"
#include "world_object.h"
#include <math.h>
#include <stdlib.h>
#include "enemy.h"
#include "bullet.h"
#include "player.h"
#include "barrel.h"

void increase_arr_bullets_capacity(world_t* world) {
    world->bullet_array.capacity = world->bullet_array.capacity * 2;
    world->bullet_array.array = realloc(world->bullet_array.array, world->bullet_array.capacity * sizeof(bullet_t));
}

void bullet_destruct(world_t* world, int index) {
    for (int i = index; i < world->bullet_array.len - 1; i++) {
        world->bullet_array.array[i] = world->bullet_array.array[i + 1];
    }
    world->bullet_array.len--;
}

void bullets_movement(world_t* world, float time_elapsed) {
    for (int i = 0; i < world->bullet_array.len; i++) {
        double new_x = world->bullet_array.array[i].pos.x;
        new_x += time_elapsed * world->bullet_array.array[i].speed * sin(world->bullet_array.array[i].angle);
        double new_y = world->bullet_array.array[i].pos.y;
        new_y += time_elapsed * world->bullet_array.array[i].speed * cos(world->bullet_array.array[i].angle);
        if (!is_wall(new_x, new_y) && !is_door(new_x, new_y)) {
            world->bullet_array.array[i].pos.x = new_x;
            world->bullet_array.array[i].pos.y = new_y;
        }
        else {
            bullet_destruct(world, i);
        }
        int index;
        if (is_enemy(world->bullet_array.array[i].pos.x, world->bullet_array.array[i].pos.y, &index)) {
            if (world->bullet_array.array[i].host == kBulletPlayer) {
                enemy_hit(world, index, world->bullet_array.array[i].damage);
                bullet_destruct(get_world(), i);
            }
        }
        if (is_player(world->bullet_array.array[i].pos.x, world->bullet_array.array[i].pos.y)) {
            if (world->bullet_array.array[i].host == kBulletEnemy) {
                player_hit(world->bullet_array.array[i].damage);
                bullet_destruct(get_world(), i);
            }
        }
        if (is_barrel(world->bullet_array.array[i].pos, &index)) {
            blow_barrel(world, index);
        }
    }
}

void shoot_bullet(world_t* world, point_t pos, double angle, float time_elapsed, bullet_host_t host, double damage, bullet_type_t type) {
    if (world->bullet_array.len >= world->bullet_array.capacity)
        increase_arr_bullets_capacity(world);
    pos.x += 0.4 * sin(angle);
    pos.y += 0.4 * cos(angle);
    world->bullet_array.array[world->bullet_array.len].pos = pos;
    world->bullet_array.array[world->bullet_array.len].angle = angle;
    world->bullet_array.array[world->bullet_array.len].speed = 4;
    world->bullet_array.array[world->bullet_array.len].radius = 0.01;
    world->bullet_array.array[world->bullet_array.len].host = host;
    world->bullet_array.array[world->bullet_array.len].damage = damage;
    world->bullet_array.array[world->bullet_array.len].type = type;
    world->bullet_array.len++;
}
