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
    } else
        q->tail->ptr = temp;
    q->tail = temp;
    temp->point = point;
    temp->ptr = NULL;
}

int isempty_point_queue(point_queue_t q) {
    if (q.head == NULL)
        return 1;
    else
        return 0;
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
