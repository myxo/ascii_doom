#define _USE_MATH_DEFINES

#include "sprite.h"
#include "world_object.h"
#include "bullet.h"
#include "weapon.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "drop.h"
#include "olc/olc.h"

void increase_arr_drop_capacity(world_t* world) {
    world->drop_array.capacity = world->drop_array.capacity * 2;
    world->drop_array.array = realloc(world->drop_array.array, world->drop_array.capacity * sizeof(drop_t));
}

void add_drop(world_t* world, point_t pos) {
    if (world->drop_array.len >= world->drop_array.capacity - 1) {
        increase_arr_drop_capacity(world);
    }
    world->drop_array.array[world->drop_array.len].type = rand() % 2;
    world->drop_array.array[world->drop_array.len].radius = 0.2;
    world->drop_array.array[world->drop_array.len].pos = pos;
    world->drop_array.len++;
    olc_play_sound(world->sound_effects.item_spawn_id);
}

void take_drop(world_t* world, int index) {
    if (world->drop_array.array[index].type == FIRST_AID) {
        world->player.health += world->first_aid_heal;
        if (world->player.health > world->player.maxhealth) {
            world->player.health = world->player.maxhealth;
        }
    }
    else if (world->drop_array.array[index].type == BULLETS) {
        if (world->weapon_list->active_weapon == PISTOL) {
            world->weapon_list->pistol->bullets += (int)world->pistol_ammo;
        }
        else if (world->weapon_list->active_weapon == RIFLE) {
            world->weapon_list->rifle->bullets += (int)world->rifle_ammo;
        }
        else {
            world->weapon_list->rocket_launcher->bullets += (int)world->rocket_ammo;
        }
    }
    for (int i = index; i < world->drop_array.len - 1; i++) {
        world->drop_array.array[i] = world->drop_array.array[i + 1];
    }
    world->drop_array.len--;
    olc_play_sound(world->sound_effects.item_pickup_id);
}

void drop_check(world_t* world) {
    for (int i = 0; i < world->drop_array.len; i++) {
        double distance_to_player = get_distance_from_pos1_to_pos2(world->drop_array.array[i].pos, world->player.pos);
        if (distance_to_player <= world->player.radius + world->drop_array.array[i].radius) {
            take_drop(world, i);
        }
    }
}

void init_drop_spawner(world_t* world) {
    world->drop_spawners[0] = get_rand_pos_on_floor(world, 1);
    world->drop_spawners[1] = get_rand_pos_on_floor(world, 1);
    world->drop_spawners[2] = get_rand_pos_on_floor(world, 1);
    world->time_for_next_drop = 0;
    world->time_since_last_drop = 0;
}

point_t get_rand_pos_around_point(point_t point) {
    point_t pos;
    do {
        pos.x = rand() % 6 - 3 + point.x;
        pos.y = rand() % 6 - 3 + point.y;
    } while (is_wall_in_radius(pos.x, pos.y, 0.1));
    return pos;
}

void drop_timer(world_t* world, double time_elapsed) {
    world->time_since_last_drop += time_elapsed;
    if (world->time_since_last_drop >= world->time_for_next_drop) {
        add_drop(world, get_rand_pos_around_point(world->drop_spawners[rand() % 3]));
        world->time_since_last_drop = 0;
        world->time_for_next_drop = rand() % 30 + 5;
    }
}
