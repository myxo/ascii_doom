#include "world_object.h"
#include "barrel.h"
#include "explosion.h"

#include <cassert>

void increase_barrel_arry_capacity(world_t* world) {
    world->barrel_array.capacity *= 2;
    world->barrel_array.array = realloc(world->barrel_array.array, world->barrel_array.capacity * sizeof(barrel_t));
}

void add_barrel(world_t* world, point_t pos, double damage, double radius) {
    if (world->barrel_array.len >= world->barrel_array.capacity)
        increase_barrel_arry_capacity(world);
    barrel_t new_barrel;
    new_barrel.pos = pos;
    new_barrel.damage = damage;
    new_barrel.radius = radius;
    world->barrel_array.array[world->barrel_array.len++] = new_barrel;
}

void delete_barrel(world_t* world, int index) {
    assert(index >= 0);
    assert(index < world->barrel_array.len);
    for (int i = index; i < len - 1; i++) {
        world->barrel_array.array[i] = world->barrel_array.array[i + 1];
    }
    len--;
}

void blow_barrel(world_t* world, int index) {
    
}
