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
    double radius;
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
    int health;
    point_t pos;
    point_t target;
    double angle_of_vision;
    double angle;
    double speed;
    double radius;
} enemy_t;

typedef struct {
    enemy_t* array;
    int len;
    int capacity;
} enemy_array_t;

typedef struct {
    player_t player;
    bullet_array_t bullet_array;
    enemy_array_t enemy_array;
    char map[16][16];
    int map_width;
    int map_height;
} world_t;


void init_world_object();
void deinit_world_object();
world_t* get_world();
int is_wall(double x, double y);
int is_bullet(double x, double y);
int is_player(double x, double y);
int is_enemy(double x, double y);

#endif
