#ifndef WORLD_OBJECT_H_
#define WORLD_OBJECT_H_

typedef struct {
    double x;
    double y;
} point_t;


typedef struct {
    point_t pos;
    double angle_of_vision;
    double angle;
    double speed;
    double angular_speed;
} player_t;

typedef struct {
    point_t pos;
    double angle;
    double speed;
    double radius;
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
    player_t player;
    char** map;
    int map_width;
    int map_height;
    bullet_array_t bullet_array;
    game_textures_t textures;
} world_t;


int init_world_object();
void deinit_world_object();
world_t* get_world();
int read_map_for_file();
int is_wall(double x, double y);
int is_bullet(double x, double y);

#endif
