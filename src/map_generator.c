#define _USE_MATH_DEFINES

#include "map_generator.h"
#include "logging.h"
#include "world_object.h"

#include <stdlib.h>
#include <math.h>

point_t* create_point_t_4cube() {
    point_t temp;
    point_t* cube = malloc(sizeof(point_t) * 12);
    temp.x = 0;
    temp.y = 0;
    cube[0] = temp;
    temp.x = 1;
    temp.y = 0;
    cube[1] = temp;
    temp.x = 2;
    temp.y = 0;
    cube[2] = temp;
    temp.x = 3;
    temp.y = 0;
    cube[3] = temp;
    temp.x = 0;
    temp.y = 1;
    cube[4] = temp;
    temp.x = 0;
    temp.y = 2;
    cube[5] = temp;
    temp.x = 0;
    temp.y = 3;
    cube[6] = temp;
    temp.x = 1;
    temp.y = 3;
    cube[7] = temp;
    temp.x = 2;
    temp.y = 3;
    cube[8] = temp;
    temp.x = 3;
    temp.y = 3;
    cube[9] = temp;
    temp.x = 3;
    temp.y = 1;
    cube[10] = temp;
    temp.x = 3;
    temp.y = 2;
    cube[11] = temp;
    return cube;
}

type_of_room_t read_room_for_file(char* str) {
    FILE* fmap;
    int width, height;
    fmap = fopen(str, "r");
    if (fmap == NULL) {
        return;
    }
    char tmp;
    fscanf(fmap, "%d %d", &width, &height);
    char** map = malloc(height * sizeof(char*));
    for (int i = 0; i < height; i++) {
        map[i] = malloc((width + 1) * sizeof(char));
    }
    fgetc(fmap);

    type_of_room_t type_of_room;
    type_of_room.height = height;
    type_of_room.width = width;
    type_of_room.shape = init_point_array(4);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            map[i][j] = fgetc(fmap);
            if (map[i][j] == '#') {
                if (type_of_room.shape.len >= type_of_room.shape.capacity)
                    increase_arr_point_capacity(&type_of_room.shape);
                point_t temp_point = { i, j };
                type_of_room.shape.array[type_of_room.shape.len++] = temp_point;
            }
        }
        map[i][width] = 0;
        fgetc(fmap);
    }
    fclose(fmap);

    if (height > width)
        type_of_room.radius = height / 2 + 1;
    else
        type_of_room.radius = width / 2 + 1;
    type_of_room.center.x = width / 2;
    type_of_room.center.y = height / 2;
    return type_of_room;
}

point_t get_random_point_on_circle(double start_angle, double stop_angle, point_t center, double r) {
    double random_num = rand();
    double angle = start_angle + random_num - (int)(random_num / (stop_angle - start_angle)) * (stop_angle - start_angle);
    point_t random_point = { center.x + r * cos(angle), center.y + r * sin(angle) };
    return random_point;
}

int is_intersection_line_with_circle(double A, double B, double C, double r) {
    double x0 = -A * C / (A * A + B * B), y0 = -B * C / (A * A + B * B);
    return C * C <= r * r * (A * A + B * B);
}

int is_intersection_circle_with_circle(point_t center1, point_t center2, double r1, double r2) {
    double x2 = center2.x - center1.x;
    double y2 = center2.y - center1.y;
    return is_intersection_line_with_circle(-2*x2, -2*y2, x2*x2 + y2*y2 + r1*r1 - r2*r2, r1);
}

void init_graph_of_rooms() {
    point_t* room = create_point_t_4cube();
    type_of_room_t type_of_room = read_room_for_file("map.txt");
    char** im = malloc(type_of_room.height * sizeof(char*));
    for (int i = 0; i < type_of_room.height; i++) {
        im[i] = malloc((type_of_room.width + 1) * sizeof(char));
        for (int j = 0; j < type_of_room.width; j++) {
            im[i][j] = ' ';
        }
        im[i][type_of_room.width] = '\0';
    }
    for (int i = 0; i < type_of_room.shape.len; i++) {
        im[(int)type_of_room.shape.array[i].x][(int)type_of_room.shape.array[i].y] = '#';
    }
    for (int i = 0; i < 16; i++) {
        fprintf(log_file(), "%s\n", im[i]);
    }
    point_t tempc1 = { 0, 0 };
    point_t tempc2 = { 6, 3 };
    point_t x = get_random_point_on_circle(0, 2 * M_PI, tempc1, 2);
}
