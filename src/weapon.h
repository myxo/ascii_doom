#ifndef WEAPON_H_
#define WEAPON_H_

void reload_gun(weapon_t* weapon, double* time_since_last_shot);

void init_std_weapon_list(std_weapon_list_t* weapon_list);

void deinit_std_weapon_list(std_weapon_list_t* weapon_list);

void init_pistol(weapon_t* pistol);

void init_rifle(weapon_t* rifle);

void init_rocket_launcher(weapon_t* rocket_launcher);

void shoot_from_active_weapon(world_t* world);

void reload_active_weapon(world_t* world);

void update_time_since_last_shot(world_t* world, float time_elapsed);

void set_active_weapon(world_t* world, enum GUN weapon);

#endif // !WEAPON_H_
