#ifndef WORLD_OBJECT_H_
#define WORLD_OBJECT_H_

typedef struct {
    double x;
    double y;
} point_t;


typedef struct {
    int health;
    point_t pos;
    double angle_of_vision;
    double angle;
    double speed;
    double angular_speed;
    double radius;
} player_t;

typedef struct {
    point_t pos;
    double angle;
    double speed;
    double radius;
    int host;
} bullet_t;

typedef struct {
    bullet_t* array;
    int len;
    int capacity;
} bullet_array_t;

typedef struct {
    sprite_t* wall;
    sprite_t* bullet;
} game_textures_t;

typedef struct {
    int health;
    point_t pos;
    point_t target;
    double angle_of_vision;
    double angle;
    double speed;
    double radius;
    double time_from_last_shot;
} enemy_t;

typedef struct {
    enemy_t* array;
    int len;
    int capacity;
} enemy_array_t;

typedef struct {
    canvas_t* main_menu;
    canvas_t* game;
} game_layouts_t;

typedef struct {
    player_t player;
    char** map;
    bullet_array_t bullet_array;
    enemy_array_t enemy_array;
    int map_width;
    int map_height;
    game_textures_t textures;
    game_layouts_t game_layouts;
} world_t;


int init_world_object();
void deinit_world_object();
world_t* get_world();
int read_map_for_file();
int is_in_circle(point_t pos, point_t circle_center, double radius);
int is_wall(double x, double y);
int is_bullet(double x, double y);
int is_player(double x, double y);
int is_enemy(double x, double y, int* enemy_index);
point_t get_rand_pos_on_floor(world_t* world);
double get_angle_from_pos1_to_pos2(point_t pos1, point_t pos2);
double get_distance_from_pos1_to_pos2(point_t pos1, point_t pos2);
int has_wall_between(point_t pos1, point_t pos2);


#endif
