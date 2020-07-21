#ifndef MAP_GENERATOR_H_
#define MAP_GENERATOR_H_

#include "world_object.h"

typedef struct{
    int width, height;
    point_array_t shape;
    point_array_t floor;
    point_array_t doors;
    int radius;
    point_t center;
} type_of_room_t;

typedef struct node_of_room {
    point_t center_on_map;
    type_of_room_t type_of_room;
    struct node_of_room** next_nodes;
    int len_nexts;
    int capacity_nexts;
} node_of_room_t;

typedef struct graph_of_rooms {
    node_of_room_t* start;
    node_of_room_t** array_of_rooms;
    int n_nodes;
    int* is_exist;
} graph_of_rooms_t;

void create_map(world_t* world);

#endif 
