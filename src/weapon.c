#include "world_object.h"
#include "weapon.h"
#include "rocket.h"
#include "weapon.h"
#include "bullet.h"
#include <stdlib.h>

void shoot_from_weapon(weapon_t* weapon, double* time_since_last_shot) {
    if (*time_since_last_shot >= weapon->shot_delay) {
        if (weapon->label != ROCKET_LAUNCHER) {
            bullet_type_t type;
            if (weapon->label == PISTOL) {
                type = player_pistol;
            }
            else {
                type = player_rifle;
            }
            shoot_bullet(get_world(), get_world()->player.pos, get_world()->player.angle, 0, weapon->host, weapon->damage, type);
        }
        else {
            shoot_rocket(get_world(), get_world()->player.pos, get_world()->player.angle, weapon->host, weapon->damage, weapon->expl_radius);
        }
        weapon->shot_delay = 1 / weapon->fire_rate;
        *time_since_last_shot = 0;
    }
}

void init_std_weapon_list(std_weapon_list_t* weapon_list) {
    weapon_list->pistol = malloc(sizeof(weapon_t));
    weapon_list->rifle = malloc(sizeof(weapon_t));
    weapon_list->rocket_launcher = malloc(sizeof(weapon_t));
    weapon_list->active_weapon = PISTOL;
    weapon_list->time_since_last_shot = 0;
    init_pistol(weapon_list->pistol);
    init_rifle(weapon_list->rifle);
    init_rocket_launcher(weapon_list->rocket_launcher);
}

void deinit_std_weapon_list(std_weapon_list_t* weapon_list) {
    free(weapon_list->pistol);
    free(weapon_list->rifle);
    free(weapon_list->rocket_launcher);
    free(weapon_list);
}

void init_pistol(weapon_t* pistol) {
    pistol->damage = 1;
    pistol->fire_rate = 1;
    pistol->host = kBulletPlayer;
    pistol->label = PISTOL;
    pistol->shot_delay = 0;
}

void init_rifle(weapon_t* rifle) {
    rifle->damage = 0.5;
    rifle->fire_rate = 5;
    rifle->label = RIFLE;
    rifle->host = kBulletPlayer;
    rifle->shot_delay = 0;
}

void init_rocket_launcher(weapon_t* rocket_launcher) {
    rocket_launcher->damage = 1.5;
    rocket_launcher->fire_rate = 0.5;
    rocket_launcher->host = kBulletPlayer;
    rocket_launcher->label = ROCKET_LAUNCHER;
    rocket_launcher->expl_radius = 3;
    rocket_launcher->shot_delay = 0;
}

void shoot_from_active_weapon(world_t* world) {
    if (world->weapon_list->active_weapon == PISTOL) {
        shoot_from_weapon(world->weapon_list->pistol, &world->weapon_list->time_since_last_shot);
    }
    else if (world->weapon_list->active_weapon == RIFLE) {
        shoot_from_weapon(world->weapon_list->rifle, &world->weapon_list->time_since_last_shot);
    }
    else if (world->weapon_list->active_weapon == ROCKET_LAUNCHER) {
        shoot_from_weapon(world->weapon_list->rocket_launcher, &world->weapon_list->time_since_last_shot);
    }
}

void update_time_since_last_shot(world_t* world, float time_elapsed) {
    world->weapon_list->time_since_last_shot += time_elapsed;
}

void set_active_weapon(world_t* world, enum GUN weapon) {
    world->weapon_list->active_weapon = weapon;
}
