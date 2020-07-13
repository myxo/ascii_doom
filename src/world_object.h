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
    player_t player;
} world_t;


void init_world_object();
void deinit_world_object();
world_t* get_world();

#endif
