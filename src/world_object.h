#ifndef WORLD_OBJECT_H_
#define WORLD_OBJECT_H_
#define MAX_BUFF 2500

#include "sprite.h"

enum PLACE_ON_SCREEN {
    FLOOR,
    AIR
};
enum GUN {
    PISTOL,
    RIFLE,
    ROCKET_LAUNCHER
};

typedef enum type_of_enemy {
    hound,
    shooter
} type_of_enemy_t;

typedef struct {
    double x;
    double y;
} point_t;

typedef struct {
    point_t* array;
    int len;
    int capacity;
} point_array_t;

typedef struct {
    double health;
    int maxhealth;
    point_t pos;
    double angle_of_vision;
    double angle;
    double speed;
    double angular_speed;
    double radius;
    double regen;
    double angular_speed_multi;
    double angular_acceleration;
} player_t;

typedef struct {
    texture_t wall;
    texture_t bullet;
    texture_t mob1;
    texture_t door;
    texture_t drop1;
    texture_t drop2;
    texture_t barrel;
} game_textures_t;

typedef struct {
    sprite_t* wall;
    sprite_t* bullet_pistol;
    sprite_t* bullet_rifle;
    sprite_t* bullet_caco;
    sprite_t* mob1;
    sprite_t* door;
    sprite_t* mob1_back;
    sprite_t* mob1_side1;
    sprite_t* mob1_side2;
    sprite_t* drop1;
    sprite_t* drop2;
    sprite_t* barrel;
} game_sprites_t;


typedef struct {
    double health;
    point_t pos;
    point_t global_target;
    point_array_t path;
    int local_target_id;
    double angle_of_vision;
    double angle;
    double speed;
    double radius;
    double time_from_last_shot;
    point_t last_player_pos;
    type_of_enemy_t type;
} enemy_t;

typedef struct {
    enemy_t* array;
    int len;
    int capacity;
} enemy_array_t;

typedef enum bullet_host {
    kBulletPlayer,
    kBulletEnemy
} bullet_host_t;

typedef enum door_status {
    door_open,
    door_close
} door_status_t;

typedef enum bullet_type {
    CACODEMON,
    PLAYER_PISTOL,
    PLAYER_RIFLE
} bullet_type_t;

typedef struct {
    sprite_t sprite;
    bullet_host_t host;
    enum GUN label;
    int bullets;
    int magazine_bullets;
    int max_magazine_bullets;
    double fire_rate;
    double damage;
    double shot_delay;
    double reload_delay;
    double expl_radius;
    int fire_sound;
} weapon_t;

typedef struct {
    point_t pos;
    double angle;
    double speed;
    double radius;
    int host;
    double damage;
    bullet_type_t type;
} bullet_t;

typedef struct {
    int type;
    double radius;
    point_t pos;
} drop_t;

typedef struct {
    drop_t* array;
    int len;
    int capacity;
} drop_array_t;

typedef struct {
    point_t pos;
    double expl_radius;
    double damage;
    double radius;
} barrel_t;

typedef struct {
    barrel_t* array;
    int len;
    int capacity;
} barrel_array_t;

typedef struct {
    point_t pos;
    door_status_t status;
    double speed_shift_movement_x;
    double speed_shift_movement_y;
} door_t;

typedef struct {
    door_t* array;
    int len;
    int capacity;
} door_array_t;

typedef struct {
    point_t pos;
    double angle;
    double speed;
    double radius;
    int host;
    double damage;
    double explosive_radius;
} rocket_t;

typedef struct {
    point_t pos;
    double radius;
    double life_time;
    double max_life_time;
} explosion_t;

typedef struct {
    explosion_t* array;
    int len;
    int capacity;
    int explosion_sound;
} explosion_array_t;



typedef struct {
    weapon_t* pistol;
    weapon_t* rifle;
    weapon_t* rocket_launcher;
    enum GUN active_weapon;
    double time_since_last_shot;
    double time_since_last_reload;
    int is_reloading;
} std_weapon_list_t;

typedef struct {
    bullet_t* array;
    int len;
    int capacity;
} bullet_array_t;

typedef struct {
    rocket_t* array;
    int len;
    int capacity;
} rocket_array_t;

typedef struct {
    int* music_array;
    float* duration_array;
    int len;
    float current_music_time;
    int current_index;
}music_array_t;

typedef struct {
    int caco_fire_sound_id;
    int caco_pain_sound_id;
    int caco_death_sound_id;
    int item_pickup_id;
    int item_spawn_id;
    int door_open_id;
    int door_close_id;
} sound_effects_t;

typedef struct {
    player_t player;
    char** map;
    bullet_array_t bullet_array;
    rocket_array_t rocket_array;
    explosion_array_t explosion_array;
    enemy_array_t enemy_array;
    std_weapon_list_t* weapon_list;
    barrel_array_t barrel_array;
    int map_width;
    int map_height;
    double** z_buffer;
    game_textures_t textures;
    game_sprites_t sprites;
    door_array_t door_array;
    music_array_t music;
    sound_effects_t sound_effects;

    drop_array_t drop_array;
    double first_aid_heal;
    double pistol_ammo;
    double rifle_ammo;
    double rocket_ammo;
    double** door_shift_map_x;
    double** door_shift_map_y;
} world_t;

void init_player(world_t* world);
int init_world_object();
void init_door_array(world_t* world, int capacity);
void deinit_world_object();
void init_z_buffer();
void deinit_z_buffer();
world_t* get_world();
int read_map_for_file();
int is_in_circle(point_t pos, point_t circle_center, double radius);
int is_wall(double x, double y);
int is_door(double x, double y);
int is_wall_in_radius(double x, double y, double radius);
int is_bullet(double x, double y);
int is_player(double x, double y);
int is_enemy(double x, double y, int* enemy_index);
int is_barrel(point_t pos, int* index);
point_t get_rand_pos_on_floor(world_t* world, double radius);
double get_angle_from_pos1_to_pos2(point_t pos1, point_t pos2);
double get_distance_from_pos1_to_pos2(point_t pos1, point_t pos2);
int has_wall_between(point_t pos1, point_t pos2);
void spawn_barrels();
point_array_t init_point_array(int capacity);
void increase_arr_point_capacity(point_array_t* point_array);
int has_wall_between_by_angle(point_t pos1, point_t pos2, double angle, double d_distance);
int has_door_between(point_t pos1, point_t pos2);
int is_door_in_radius(double x, double y, double radius);
void update_world_from_config();
void update_doors_status(world_t* world);


#endif
