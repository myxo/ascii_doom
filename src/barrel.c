#include "world_object.h"
#include "barrel.h"
#include "explosion.h"

#include <cassert>
#include <stdlib.h>

void increase_barrel_arry_capacity (world_t* world) {
    world->barrel_array.capacity *= 2;
    world->barrel_array.array = realloc(world->barrel_array.array, world->barrel_array.capacity * sizeof(barrel_t));
}

void add_barrel(world_t* world, point_t pos, double damage, double expl_radius, double radius) {
    if (world->barrel_array.len >= world->barrel_array.capacity)
        increase_barrel_arry_capacity(world);
    barrel_t new_barrel;
    new_barrel.pos = pos;
    new_barrel.damage = damage;
    new_barrel.radius = radius;
    new_barrel.expl_radius = expl_radius;
    world->barrel_array.array[world->barrel_array.len++] = new_barrel;
}

void delete_barrel(world_t* world, int index) {
    assert(index >= 0);
    assert(index < world->barrel_array.len);
    for (int i = index; i < world->barrel_array.len - 1; i++) {
        world->barrel_array.array[i] = world->barrel_array.array[i + 1];
    }
    world->barrel_array.len--;
}

void blow_barrel(world_t* world, int index) {
    point_t pos = world->barrel_array.array[index].pos;
    double damage = world->barrel_array.array[index].damage;
    double expl_radius = world->barrel_array.array[index].expl_radius;
    delete_barrel(world, index);
    make_explosion(world, pos, damage, expl_radius);
}
