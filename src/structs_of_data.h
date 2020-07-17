#ifndef STRUCTS_OF_DATA_H_
#define STRUCTS_OF_DATA_H_

#include "world_object.h"

typedef struct point_list {
    point_t point;
    struct point_list* ptr;
} point_list_t;

typedef struct {
    point_list_t* head, * tail;
} point_queue_t;

point_queue_t point_queue_init();
int isempty_point_queue(point_queue_t q);
void point_queue_push_back(point_queue_t* q, point_t point);
point_t point_queue_pop(point_queue_t* q);

#endif
