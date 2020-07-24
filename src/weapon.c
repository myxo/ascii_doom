#include "world_object.h"
#include "weapon.h"
#include "rocket.h"
#include "weapon.h"
#include "bullet.h"
#include "olc/olc.h"
#include <stdlib.h>

void reload_gun(weapon_t* weapon, double* time_since_reload) {
    if (*time_since_reload >= weapon->reload_delay) {
        if (weapon->bullets > 0) {
            if (weapon->bullets >= weapon->max_magazine_bullets) {
                weapon->bullets -= weapon->max_magazine_bullets - weapon->magazine_bullets;
                weapon->magazine_bullets = weapon->max_magazine_bullets;
            }
            else {
                weapon->magazine_bullets += weapon->bullets;
                weapon->bullets = 0;
                if (weapon->magazine_bullets > weapon->max_magazine_bullets) {
                    weapon->bullets = weapon->magazine_bullets - weapon->max_magazine_bullets;
                    weapon->magazine_bullets = weapon->max_magazine_bullets;
                }
            }
        }
        get_world()->weapon_list->is_reloading = 0;
    }
}

void shoot_from_weapon(weapon_t* weapon, double* time_since_last_shot) {
    if (*time_since_last_shot >= weapon->shot_delay) {
        if (weapon->magazine_bullets > 0) {
            weapon->magazine_bullets--;
            if (weapon->label != ROCKET_LAUNCHER) {
              bullet_type_t type;
              if (weapon->label == PISTOL) {
                type = PLAYER_PISTOL;
              }
              else {
                type = PLAYER_RIFLE;
              }
              shoot_bullet(get_world(), get_world()->player.pos, get_world()->player.angle, 0, weapon->host, weapon->damage, type);
            }
            else {
                shoot_rocket(get_world(), get_world()->player.pos, get_world()->player.angle, weapon->host, weapon->damage, weapon->expl_radius);
            }
            play_sound(weapon->fire_sound);
            weapon->shot_delay = 1 / weapon->fire_rate;
            *time_since_last_shot = 0;
        }
        else {
            get_world()->weapon_list->time_since_last_reload = 0;
            get_world()->weapon_list->is_reloading = 1;
        }
    }
}

void init_std_weapon_list(std_weapon_list_t* weapon_list) {
    weapon_list->pistol = malloc(sizeof(weapon_t));
    weapon_list->rifle = malloc(sizeof(weapon_t));
    weapon_list->rocket_launcher = malloc(sizeof(weapon_t));
    weapon_list->active_weapon = PISTOL;
    weapon_list->time_since_last_shot = 0;
    weapon_list->time_since_last_reload = 0;
    weapon_list->is_reloading = 0;
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
    pistol->damage = 34;
    pistol->fire_rate = 1;
    pistol->bullets = 5;
    pistol->magazine_bullets = 10;
    pistol->max_magazine_bullets = 10;
    pistol->host = kBulletPlayer;
    pistol->label = PISTOL;
    pistol->shot_delay = 0;
    pistol->reload_delay = 4 / pistol->fire_rate;
    pistol->fire_sound = olc_load_sound("dspistol.wav");
}

void init_rifle(weapon_t* rifle) {
    rifle->damage = 16;
    rifle->fire_rate = 5;
    rifle->bullets = 0;
    rifle->magazine_bullets = 30;
    rifle->max_magazine_bullets = 30;
    rifle->label = RIFLE;
    rifle->host = kBulletPlayer;
    rifle->shot_delay = 0;
    rifle->reload_delay = 4 / rifle->fire_rate;
    rifle->fire_sound = olc_load_sound("dspistol.wav");
}

void init_rocket_launcher(weapon_t* rocket_launcher) {
    rocket_launcher->damage = 50;
    rocket_launcher->fire_rate = 0.5;
    rocket_launcher->bullets = 4;
    rocket_launcher->magazine_bullets = 1;
    rocket_launcher->max_magazine_bullets = 1;
    rocket_launcher->host = kBulletPlayer;
    rocket_launcher->label = ROCKET_LAUNCHER;
    rocket_launcher->expl_radius = 3;
    rocket_launcher->shot_delay = 0;
    rocket_launcher-> reload_delay = 4 / rocket_launcher->fire_rate;
    rocket_launcher->fire_sound = olc_load_sound("dsrlaunc.wav");
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

void reload_active_weapon(world_t* world) {
    if (world->weapon_list->active_weapon == PISTOL) {
        reload_gun(world->weapon_list->pistol, &world->weapon_list->time_since_last_reload);
    }
    else if (world->weapon_list->active_weapon == RIFLE) {
        reload_gun(world->weapon_list->rifle, &world->weapon_list->time_since_last_reload);
    }
    else if (world->weapon_list->active_weapon == ROCKET_LAUNCHER) {
        reload_gun(world->weapon_list->rocket_launcher, &world->weapon_list->time_since_last_reload);
    }
}

void update_time_since_last_shot(world_t* world, float time_elapsed) {
    world->weapon_list->time_since_last_shot += time_elapsed;
}

void update_time_since_reload(world_t* world, float time_elapsed) {
    world->weapon_list->time_since_last_reload += time_elapsed;
}

void set_active_weapon(world_t* world, enum GUN weapon) {
    world->weapon_list->active_weapon = weapon;
}

weapon_t* get_active_weapon(world_t* world) {
    if (world->weapon_list->active_weapon == PISTOL) {
        return world->weapon_list->pistol;
    }
    else if (world->weapon_list->active_weapon == RIFLE) {
        return world->weapon_list->rifle;
    }
    else if (world->weapon_list->active_weapon == ROCKET_LAUNCHER) {
        return world->weapon_list->rocket_launcher;
    }
    return world->weapon_list->rocket_launcher;
}

void check_reload(weapon_t* weapon, world_t* world, double time_elapsed) {
    if (weapon->magazine_bullets <= 0 && weapon->bullets > 0) {
        world->weapon_list->is_reloading = 1;
    }
    if (world->weapon_list->is_reloading == 1) {
        update_time_since_reload(world, time_elapsed);
        reload_active_weapon(world);
    }
}
