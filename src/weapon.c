#include "world_object.h"
#include "weapon.h"
#include "bullet.h"
#include <stdlib.h>

void shoot_from_weapon(weapon_t* weapon) {
    if (!weapon->is_burst_fire) {
        if (weapon->time_since_last_shoot >= weapon->shot_delay) {
            shoot_bullet(get_world(), get_world()->player.pos, get_world()->player.angle, 0, weapon->host, weapon->damage);
            weapon->time_since_last_shoot = 0;
            weapon->shot_delay = 1 / weapon->fire_rate;
        }
    }
    else {
        double time_delay;
        if (weapon->time_since_last_shoot >= weapon->shot_delay) {
            shoot_bullet(get_world(), get_world()->player.pos, get_world()->player.angle, 0, weapon->host, weapon->damage);
            weapon->time_since_last_shoot = 0;
            weapon->shot_made_in_burst++;
            if (weapon->shot_made_in_burst == weapon->burst_size) {
                weapon->shot_made_in_burst = 0;
                weapon->shot_delay = 1 / weapon->fire_rate;
            }
            else {
                weapon->shot_delay = 1 / weapon->fire_rate_in_burst;
            }
        }
    }
}

void init_std_weapon_list(std_weapon_list_t* weapon_list) {
    weapon_list->pistol = malloc(sizeof(weapon_t));
    weapon_list->rifle = malloc(sizeof(weapon_t));
    init_pistol(weapon_list->pistol);
    init_rifle(weapon_list->rifle);
}

void init_pistol(weapon_t* pistol) {
    pistol->is_burst_fire = 0;
    pistol->damage = 1;
    pistol->fire_rate = 1;
    pistol->host = kBulletPlayer;
    pistol->time_since_last_shoot = 0;
    pistol->is_active = 1;
    pistol->shot_delay = 0;
}

void init_rifle(weapon_t* rifle) {
    rifle->is_burst_fire = 1;
    rifle->damage = 0.5;
    rifle->burst_size = 3;
    rifle->fire_rate_in_burst = 6;
    rifle->fire_rate = 0.5;
    rifle->host = kBulletPlayer;
    rifle->time_since_last_shoot = 0;
    rifle->shot_made_in_burst = 0;
    rifle->is_active = 0;
    rifle->shot_delay = 0;
}

void shoot_from_active_weapon(world_t* world) {
    if (world->weapon_list->pistol->is_active) {
        shoot_from_weapon(world->weapon_list->pistol);
    }
    else if (world->weapon_list->rifle->is_active) {
        shoot_from_weapon(world->weapon_list->rifle);
    }
}

void update_time_since_last_shot(world_t* world, float time_elapsed) {
    if (world->weapon_list->pistol->is_active) {
        world->weapon_list->pistol->time_since_last_shoot += time_elapsed;
    }
    else if (world->weapon_list->rifle->is_active) {
        world->weapon_list->rifle->time_since_last_shoot += time_elapsed;
    }
}

void turn_off_all_weapons(world_t* world) {
    world->weapon_list->pistol->is_active = 0;
    world->weapon_list->rifle->is_active = 0;
}

void set_active_weapon(world_t* world, char* weapon) {
    turn_off_all_weapons(world);
    if (strcmp(weapon, "pistol") == 0) {
        world->weapon_list->pistol->is_active = 1;
    }
    else if (strcmp(weapon, "rifle") == 0) {
        world->weapon_list->rifle->is_active = 1;
    }
}
