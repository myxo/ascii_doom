#ifndef WORLD_OBJECT_H_
#define WORLD_OBJECT_H_
#define MAX_BUFF 2500

#include "sprite.h"

enum GUN {
    PISTOL,
    RIFLE,
    ROCKET_LAUNCHER
};

typedef enum {
    kBulletPlayer,
    kBulletEnemy
} bullet_host_t;

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
} player_t;

typedef struct {
    texture_t wall;
    texture_t bullet;
    texture_t mob1;
    texture_t drop1;
    texture_t drop2;
} game_textures_t;

typedef struct {
    sprite_t* wall;
    sprite_t* bullet;
    sprite_t* mob1;
    sprite_t* drop1;
    sprite_t* drop2;
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
} enemy_t;

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
} weapon_t;

typedef struct {
    enemy_t* array;
    int len;
    int capacity;
} enemy_array_t;

typedef struct {
    point_t pos;
    double angle;
    double speed;
    double radius;
    int host;
    double damage;
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
    player_t player;
    char** map;
    bullet_array_t bullet_array;
    rocket_array_t rocket_array;
    explosion_array_t explosion_array;
    enemy_array_t enemy_array;
    std_weapon_list_t* weapon_list;
    int map_width;
    int map_height;
    double** z_buffer;
    game_textures_t textures;
    game_sprites_t sprites;
    drop_array_t drop_array;
    double first_aid_heal;
    double pistol_ammo;
    double rifle_ammo;
    double rocket_ammo;
} world_t;

void init_player(world_t* world);
int init_world_object();
void deinit_world_object();
void init_z_buffer();
void deinit_z_buffer();
world_t* get_world();
int read_map_for_file();
int is_in_circle(point_t pos, point_t circle_center, double radius);
int is_wall(double x, double y);
int is_wall_in_radius(double x, double y, double radius);
int is_bullet(double x, double y);
int is_player(double x, double y);
int is_enemy(double x, double y, int* enemy_index);
point_t get_rand_pos_on_floor(world_t* world, double radius);
double get_angle_from_pos1_to_pos2(point_t pos1, point_t pos2);
double get_distance_from_pos1_to_pos2(point_t pos1, point_t pos2);
int has_wall_between(point_t pos1, point_t pos2);
point_array_t init_point_array(int capacity);
void increase_arr_point_capacity(point_array_t* point_array);
int has_wall_between_by_angle(point_t pos1, point_t pos2, double angle, double d_distance);
void update_world_from_config();

#endif
