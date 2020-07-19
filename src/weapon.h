#ifndef WEAPON_H_
#define WEAPON_H_

enum GUN {
    PISTOL,
    RIFLE
};

void init_std_weapon_list(std_weapon_list_t* weapon_list);

void deinit_std_weapon_list(std_weapon_list_t* weapon_list);

void init_pistol(weapon_t* pistol);

void init_rifle(weapon_t* rifle);

void shoot_from_active_weapon(world_t* world);

void update_time_since_last_shot(world_t* world, float time_elapsed);

void turn_off_all_weapons(world_t* world);

void set_active_weapon(world_t* world, char* weapon);

#endif // !WEAPON_H_
