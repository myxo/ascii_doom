#include <stdlib.h>
#include "world_object.h"
#include "structs_of_data.h"

point_queue_t point_queue_init() {
    point_queue_t q;
    q.head = NULL;
    q.tail = NULL;
    return q;
}

void point_queue_push_back(point_queue_t* q, point_t point)
{
    point_list_t* temp = malloc(sizeof(point_list_t));
    if (q->tail == NULL || q->head == NULL) {
        q->head = temp;
    }
    else {
        q->tail->ptr = temp;
    }
    q->tail = temp;
    temp->point = point;
    temp->ptr = NULL;
}

void point_queue_push_front(point_queue_t* q, point_t point)
{
    point_list_t* temp = malloc(sizeof(point_list_t));
    temp->ptr = q->head;
    q->head = temp;
    temp->point = point;
}

int isempty_point_queue(point_queue_t q) {
    return q.head == NULL;
}


point_t point_queue_pop(point_queue_t* q) {
    point_list_t* temp;
    point_t point;
    point = q->head->point;
    temp = q->head;
    q->head = q->head->ptr;
    free(temp);
    return point;
}

void point_queue_destruct(point_queue_t* q) {
    while (!isempty_point_queue(*q)) {
        point_queue_pop(q);
    }
    free(q->tail);
}
