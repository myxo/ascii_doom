#include "world_object.h"
#include "player.h"
#include "enemy.h"
#include "explosion.h"
#include "barrel.h"
#include "olc/olc.h"

#include <stdlib.h>

void increase_explosion_array_capacity(world_t* world) {
    world->explosion_array.capacity *= 2;
    world->explosion_array.array = realloc(world->explosion_array.array, world->explosion_array.capacity * sizeof(explosion_t));
}

void add_explosion(world_t* world, point_t pos, double radius, double max_life_time) {
    if (world->explosion_array.len >= world->explosion_array.capacity)
        increase_explosion_array_capacity(world);
    world->explosion_array.array[world->explosion_array.len].pos = pos;
    world->explosion_array.array[world->explosion_array.len].radius = radius;
    world->explosion_array.array[world->explosion_array.len].max_life_time = max_life_time;
    world->explosion_array.array[world->explosion_array.len].life_time = 0;
    world->explosion_array.len++;
}

void remove_explosion(world_t* world, int index) {
    for (int i = index; i < world->explosion_array.len - 1; i++) {
        world->explosion_array.array[i] = world->explosion_array.array[i + 1];
    }
    world->explosion_array.len--;
}

void make_explosion(world_t* world, point_t pos, double std_damage, double radius) {
    if (is_in_circle(world->player.pos, pos, radius)) {
        double rate_of_damage = (radius - get_distance_from_pos1_to_pos2(world->player.pos, pos)) / radius;
        double damage = std_damage * rate_of_damage;
        player_hit(damage);
    }
    for (int i = 0; i < world->enemy_array.len; i++) {
        if (is_in_circle(world->enemy_array.array[i].pos, pos, radius)) {
            double rate_of_damage = (radius - get_distance_from_pos1_to_pos2(world->enemy_array.array[i].pos, pos)) / radius;
            double damage = std_damage * rate_of_damage;
            enemy_hit(world, i, damage);
        }
    }
    for (int i = 0; i < world->barrel_array.len; i++) {
        if (is_in_circle(world->barrel_array.array[i].pos, pos, radius)) {
            blow_barrel(world, i);
        }
    }
    add_explosion(world, pos, radius, 0.2);
    if (!world->is_mute)
        olc_play_sound(world->explosion_array.explosion_sound);
}

void update_life_time(world_t* world, float time_elapsed) {
    for (int i = 0; i < world->explosion_array.len; i++) {
        world->explosion_array.array[i].life_time += time_elapsed;
        if (world->explosion_array.array[i].life_time > world->explosion_array.array[i].max_life_time) {
            remove_explosion(world, i);
        }
    }
}
