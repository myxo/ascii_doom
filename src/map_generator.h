#ifndef MAP_GENERATOR_H_
#define MAP_GENERATOR_H_

#include "world_object.h"



typedef struct{
    int width, height;
    point_array_t shape;
    int radius;
    point_t center;
} type_of_room_t;

typedef struct node_of_room {
    type_of_room_t type_of_room;
    struct node_of_room** next_nodes;
} node_of_room_t;

typedef struct graph_of_rooms {
    node_of_room_t* start;
} graph_of_rooms_t;

void init_graph_of_rooms();

#endif 
