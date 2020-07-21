#include "world_object.h"
#include "rocket.h"
#include "render.h"
#include "explosion.h"
#include "barrel.h"

#include <stdlib.h>
#include <math.h>

void increase_arr_rockets_capacity(world_t* world) {
    world->rocket_array.capacity = world->rocket_array.capacity * 2;
    world->rocket_array.array = realloc(world->rocket_array.array, world->rocket_array.capacity * sizeof(bullet_t));
}

void rocket_destruct(world_t* world, int index) {
    for (int i = index; i < world->rocket_array.len - 1; i++) {
        world->rocket_array.array[i] = world->rocket_array.array[i + 1];
    }
    world->rocket_array.len--;
}

void rockets_movement(world_t* world, float time_elapsed) {
    for (int i = 0; i < world->rocket_array.len; i++) {
        double new_x = world->rocket_array.array[i].pos.x;
        new_x += time_elapsed * world->rocket_array.array[i].speed * sin(world->rocket_array.array[i].angle);
        double new_y = world->rocket_array.array[i].pos.y;
        new_y += time_elapsed * world->rocket_array.array[i].speed * cos(world->rocket_array.array[i].angle);
        if (!is_wall(new_x, new_y)) {
            world->rocket_array.array[i].pos.x = new_x;
            world->rocket_array.array[i].pos.y = new_y;
        }
        else {
            point_t expl_point = { new_x, new_y };
            make_explosion(world, expl_point, world->rocket_array.array[i].damage, world->rocket_array.array[i].explosive_radius);
            rocket_destruct(world, i);
        }
        int index;
        if (is_enemy(world->rocket_array.array[i].pos.x, world->rocket_array.array[i].pos.y, &index)) {
            if (world->rocket_array.array[i].host == kBulletPlayer) {
                make_explosion(world, world->rocket_array.array[i].pos, world->rocket_array.array[i].damage, world->rocket_array.array[i].explosive_radius);
                rocket_destruct(world, i);
            }
        }
        if (is_player(world->rocket_array.array[i].pos.x, world->rocket_array.array[i].pos.y)) {
            if (world->rocket_array.array[i].host == kBulletEnemy) {
                make_explosion(world, world->rocket_array.array[i].pos, world->rocket_array.array[i].damage, world->rocket_array.array[i].explosive_radius);
                rocket_destruct(world, i);
            }
        }
        if (is_barrel(world->rocket_array.array[i].pos, &index)) {
            blow_barrel(world, index);
        }
    }
}

void shoot_rocket(world_t* world, point_t pos, double angle, bullet_host_t host, double damage, double expl_radius) {
    if (world->rocket_array.len >= world->rocket_array.capacity)
        increase_arr_rockets_capacity(world);
    pos.x += 0.4 * sin(angle);
    pos.y += 0.4 * cos(angle);
    world->rocket_array.array[world->rocket_array.len].pos = pos;
    world->rocket_array.array[world->rocket_array.len].angle = angle;
    world->rocket_array.array[world->rocket_array.len].speed = 4;
    world->rocket_array.array[world->rocket_array.len].radius = 0.01;
    world->rocket_array.array[world->rocket_array.len].host = host;
    world->rocket_array.array[world->rocket_array.len].damage = damage;
    world->rocket_array.array[world->rocket_array.len].explosive_radius = expl_radius;
    world->rocket_array.len++;
}
