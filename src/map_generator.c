#define _USE_MATH_DEFINES

#include "map_generator.h"
#include "logging.h"
#include "world_object.h"
#include "structs_of_data.h"

#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>

void increase_arr_next_nodes_capacity(node_of_room_t* node) {
    node->capacity_nexts = node->capacity_nexts * 2;
    node->next_nodes = realloc(node->next_nodes, node->capacity_nexts * sizeof(node_of_room_t));
}

node_of_room_t* init_room_node(int capacity) {
    node_of_room_t* node = malloc(sizeof(node_of_room_t));
    node->capacity_nexts = capacity;
    node->len_nexts = 0;
    node->next_nodes = malloc(node->capacity_nexts * sizeof(node_of_room_t));
    return node;
}


type_of_room_t read_room_for_file(char* name_file) {
    FILE* fmap;
    int width, height;
    fmap = fopen(name_file, "r");
    char tmp;
    fscanf(fmap, "%d %d", &width, &height);
    type_of_room_t type_of_room;
    type_of_room.height = height;
    type_of_room.width = width;
    type_of_room.shape = init_point_array(4);
    type_of_room.floor = init_point_array(4);
    type_of_room.doors = init_point_array(4);
    if (fmap == NULL) {
        return type_of_room;
    }
    char** map = malloc(height * sizeof(char*));
    for (int i = 0; i < height; i++) {
        map[i] = malloc((width + 1) * sizeof(char));
    }
    fgetc(fmap);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            map[i][j] = fgetc(fmap);
            if (map[i][j] == '#') {
                if (type_of_room.shape.len >= type_of_room.shape.capacity)
                    increase_arr_point_capacity(&type_of_room.shape);
                point_t temp_point = { i, j };
                type_of_room.shape.array[type_of_room.shape.len++] = temp_point;
            }
            if (map[i][j] == '.') {
                if (type_of_room.floor.len >= type_of_room.floor.capacity)
                    increase_arr_point_capacity(&type_of_room.floor);
                point_t temp_point = { i, j };
                type_of_room.floor.array[type_of_room.floor.len++] = temp_point;
            }
            if (map[i][j] == 'd') {
                if (type_of_room.doors.len >= type_of_room.doors.capacity)
                    increase_arr_point_capacity(&type_of_room.doors);
                point_t temp_point = { i, j };
                type_of_room.doors.array[type_of_room.doors.len++] = temp_point;
            }
        }
        map[i][width] = 0;
        fgetc(fmap);
    }
    fclose(fmap);
    if (width > height)
        type_of_room.radius = width / 2;
    else
        type_of_room.radius = height / 2;
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

graph_of_rooms_t read_graph_from_file(char* name_file) {
    type_of_room_t type_of_room = read_room_for_file("main_room.txt"); //TODO
    FILE* fgraph = fopen(name_file, "r");
    int n;
    fscanf(fgraph, "%d", &n);
    init_door_array(get_world(), n * 2);
    node_of_room_t** array_of_rooms = malloc((n + 1) * sizeof(node_of_room_t*));
    int* is_exist = malloc((n + 1) * sizeof(int));
    for (int i = 0; i < n + 1; i++)
        is_exist[i] = 0;
    int pred_id, next_id;
    char str_temp[128];
    char name_type[64];
    char name_second[64];
    fgets(str_temp, 128, fgraph);
    fgets(str_temp, 128, fgraph);
    node_of_room_t* temp_node = init_room_node(3);
    node_of_room_t* temp_node_second = init_room_node(3);
    sscanf(str_temp, "%d %d %s %s", &pred_id, &next_id, name_type, name_second);
    strcat(name_type, ".txt");
    strcat(name_second, ".txt");
    temp_node->type_of_room = read_room_for_file(name_type);
    strcpy(temp_node->type_of_room.name, name_type);
    if (!strcmp(name_type, name_second))
        temp_node_second->type_of_room = temp_node->type_of_room;
    else
        temp_node_second->type_of_room = read_room_for_file(name_second);
    strcpy(temp_node_second->type_of_room.name, name_second);
    graph_of_rooms_t graph;
    graph.start = temp_node;
    temp_node->next_nodes[temp_node->len_nexts++] = temp_node_second;
    array_of_rooms[pred_id] = temp_node;
    is_exist[pred_id] = 1;
    array_of_rooms[next_id] = temp_node_second;
    is_exist[next_id] = 1;
    for (int i = 1; i  < n; i++) {
        fscanf(fgraph, "%d %d %s", &pred_id, &next_id, name_type);
        strcat(name_type, ".txt");
        if (array_of_rooms[pred_id]->len_nexts >= array_of_rooms[pred_id]->capacity_nexts)
            increase_arr_next_nodes_capacity(array_of_rooms[pred_id]);
        if (is_exist[next_id]) {
            array_of_rooms[pred_id]->next_nodes[array_of_rooms[pred_id]->len_nexts++] = array_of_rooms[next_id];
        }
        else {
            node_of_room_t* temp = init_room_node(3);
            int is_type = 0;
            for (int j = 0; j < n; j++) {
                if (is_exist[j]) {
                    if (!strcmp(array_of_rooms[j]->type_of_room.name, name_type)) {
                        temp->type_of_room = array_of_rooms[j]->type_of_room;
                        is_type = 1;
                        break;
                    }
                }
            }
            if (!is_type)
                temp->type_of_room = read_room_for_file(name_type);
            array_of_rooms[pred_id]->next_nodes[array_of_rooms[pred_id]->len_nexts++] = temp;
            array_of_rooms[next_id] = temp;
            is_exist[next_id] = 1;
        }
    }
    graph.array_of_rooms = array_of_rooms;
    graph.n_nodes = n + 1;
    graph.is_exist = is_exist;
    return graph;
}

void set_node_near(graph_of_rooms_t* graph, node_of_room_t* parent, node_of_room_t* node) {
    double external_r = node->type_of_room.radius;
    while (1) {
        point_t random_point_to_set = get_random_point_on_circle(0, 2 * M_PI, parent->center_on_map, parent->type_of_room.radius + external_r + 1);
        int count_of_check = 0;
        int is_intersection = 0;
        for (int i = 0; i < graph->n_nodes; i++) {
            if (graph->is_exist[i] && graph->array_of_rooms[i] != node) {
                is_intersection = is_intersection_circle_with_circle(graph->array_of_rooms[i]->center_on_map, random_point_to_set, parent->type_of_room.radius, node->type_of_room.radius);
                if (is_intersection)
                    break;
            }
        }
        if (is_intersection) {
            count_of_check++;
        }
        else {
            node->center_on_map = random_point_to_set;
            for (int i = 0; i < node->len_nexts; i++) {
                set_node_near(graph, node, node->next_nodes[i]);
            }
            break;
        }
        if (count_of_check % 5 == 0)
            external_r *= 2;
    }

}

char** build_corridor(graph_of_rooms_t* g, char** map, node_of_room_t* start_room, node_of_room_t* stop_room, double shift_y, double shift_x, double map_width, double map_height) {
    char ch_door_start = '0', ch_door_stop = '0';
    for (int i = 0; i < g->n_nodes; i++) {
        if (g->is_exist[i]) {
            if (g->array_of_rooms[i] == start_room)
                ch_door_start = '0' + i;
            if (g->array_of_rooms[i] == stop_room)
                ch_door_stop = '0' + i;
        }
    }

    point_queue_t q = point_queue_init();
    int** used = malloc((int)map_width * sizeof(int*));
    for (int i = 0; i < map_width; i++) {
        used[i] = malloc((int)map_height * sizeof(int));
        for (int j = 0; j < map_height; j++) {
            used[i][j] = 0;
        }
    }
    point_t** pred = malloc((int)map_width * sizeof(point_t*));
    for (int i = 0; i < map_width; i++) {
        pred[i] = malloc((int)map_height * sizeof(point_t));
    }
    for (int i = 0; i < start_room->type_of_room.doors.len; i++) {
        point_t temp;
        temp.y = start_room->center_on_map.y + start_room->type_of_room.doors.array[i].y - start_room->type_of_room.center.y + shift_y;
        temp.x = start_room->center_on_map.x + start_room->type_of_room.doors.array[i].x - start_room->type_of_room.center.x + shift_x;
        point_queue_push_back(&q, temp);
        used[(int)temp.x][(int)temp.y] = 1;
        point_t null = { -1, -1 };
        pred[(int)temp.x][(int)temp.y] = null;
    }
    point_t stop_door = {0, 0};
    int found_corridor = 0;
    while (!isempty_point_queue(q)) {
        point_t cur = point_queue_pop(&q);
        int near_corridor = 0;
        int x_move[4] = { -1, 0,  0, 1 };
        int y_move[4] = { 0, 1, -1, 0 };
        for (int i = 0; i < 4; i++) {
            int x = (int)cur.x + x_move[i];
            int y = (int)cur.y + y_move[i];
            if (x >= 0 && y >= 0 && x < map_width && y < map_height && map[y][x] == '.') {
                near_corridor = 1;
                break;
            }
        }
        if (!near_corridor) {
            for (int i = 0; i < 4; i++) {
                int x = (int)cur.x + x_move[i];
                int y = (int)cur.y + y_move[i];
                if (x >= 0 && y >= 0 && x < map_width && y < map_height && map[y][x] != '|') {
                    point_t to = { x, y };
                    if (!used[x][y]) {
                        used[x][y] = 1;
                        pred[x][y] = cur;
                        if (map[y][x] == ch_door_stop) {
                            stop_door.y = y;
                            stop_door.x = x;
                            found_corridor = 1;
                            break;
                        }
                        if (map[y][x] == '#')
                            point_queue_push_back(&q, to);
                    }
                }
            }
        }
        if (found_corridor)
            break;
    }
    point_t cur;
    char marker = ' ';
    door_t temp_door;
    temp_door.pos.x = stop_door.y;
    temp_door.pos.y = stop_door.x;
    temp_door.status = door_close;
    get_world()->door_array.array[get_world()->door_array.len++] = temp_door;
    for (cur = stop_door; !(cur.x == -1 && cur.y == -1); cur = pred[(int)cur.x][(int)cur.y]) {
        if (cur.x < 0 || cur.y < 0)
            break;
        if (map[(int)cur.y][(int)cur.x] == ch_door_stop) {
            marker = '.';
            map[(int)cur.y][(int)cur.x] = marker;
        }else if (map[(int)cur.y][(int)cur.x] == ch_door_start) {
            map[(int)cur.y][(int)cur.x] = marker;
            marker = ' ';
        }
        else {
            map[(int)cur.y][(int)cur.x] = marker;
        }
        if (pred[(int)cur.x][(int)cur.y].x == -1 && pred[(int)cur.x][(int)cur.y].y == -1) {
            temp_door;
            temp_door.pos.x = cur.y;
            temp_door.pos.y = cur.x;
            temp_door.status = door_close;
            get_world()->door_array.array[get_world()->door_array.len++] = temp_door;
        }
    }
    return map;
}

char** put_node_rooms_on_map_from_graph(graph_of_rooms_t* graph, int* width, int* height, point_t* player_pos) {
    graph->start->center_on_map.x = 0;
    graph->start->center_on_map.y = 0;
    for (int i = 0; i < graph->start->len_nexts; i++) {
        set_node_near(graph, graph->start, graph->start->next_nodes[i]);
    }

    int shift_x, shift_y;
    int min_x = INT_MAX, max_x = INT_MIN;
    int min_y = INT_MAX, max_y = INT_MIN;
    int min_x_r = 0, max_x_r = 0;
    int min_y_r = 0, max_y_r = 0;
    for (int i = 0; i < graph->n_nodes; i++) {
        if (graph->is_exist[i]) {
            if (graph->array_of_rooms[i]->center_on_map.x - graph->array_of_rooms[i]->type_of_room.radius < min_x - min_x_r) {
                min_x = (int)graph->array_of_rooms[i]->center_on_map.x;
                min_x_r = (int)graph->array_of_rooms[i]->type_of_room.radius + 1;
            }
            if (graph->array_of_rooms[i]->center_on_map.y - graph->array_of_rooms[i]->type_of_room.radius < min_y - min_y_r) {
                min_y = (int)graph->array_of_rooms[i]->center_on_map.y;
                min_y_r = (int)graph->array_of_rooms[i]->type_of_room.radius + 1;
            }
            if (graph->array_of_rooms[i]->center_on_map.x + graph->array_of_rooms[i]->type_of_room.radius > max_x + max_x_r) {
                max_x = (int)graph->array_of_rooms[i]->center_on_map.x;
                max_x_r = (int)graph->array_of_rooms[i]->type_of_room.radius + 1;
            }
            if (graph->array_of_rooms[i]->center_on_map.y + graph->array_of_rooms[i]->type_of_room.radius > max_y + max_y_r) {
                max_y = (int)graph->array_of_rooms[i]->center_on_map.y;
                max_y_r = (int)graph->array_of_rooms[i]->type_of_room.radius + 1;
            }
        }
    }
    *width = max_x - min_x + max_x_r + min_x_r;
    *height = max_y - min_y + max_y_r + min_y_r;
    shift_x = 0 - (min_x - min_x_r);
    shift_y = 0 - (min_y - min_y_r);

    char** map = malloc(*height * sizeof(char*));
    for (int i = 0; i < *height; i++) {
        map[i] = malloc((*width + 1) * sizeof(char));
        for (int j = 0; j < *width; j++)
            map[i][j] = '#';
        map[i][*width] = '\0';
    }
    for (int node_room_index = 0; node_room_index < graph->n_nodes; node_room_index++) {
        if (!graph->is_exist[node_room_index])
            continue;
        node_of_room_t node_room = *(graph->array_of_rooms[node_room_index]);
        for (int i = 0; i < node_room.type_of_room.shape.len; i++) {
            int y = (int)(node_room.center_on_map.y + node_room.type_of_room.shape.array[i].y - node_room.type_of_room.center.y + shift_y);
            int x = (int)(node_room.center_on_map.x + node_room.type_of_room.shape.array[i].x - node_room.type_of_room.center.x + shift_x);
            map[y][x] = '|';
        }
        for (int i = 0; i < node_room.type_of_room.floor.len; i++) {
            int y = (int)(node_room.center_on_map.y + node_room.type_of_room.floor.array[i].y - node_room.type_of_room.center.y + shift_y);
            int x = (int)(node_room.center_on_map.x + node_room.type_of_room.floor.array[i].x - node_room.type_of_room.center.x + shift_x);
            map[y][x] = ' ';
        }
        for (int i = 0; i < node_room.type_of_room.doors.len; i++) {
            int y = (int)(node_room.center_on_map.y + node_room.type_of_room.doors.array[i].y - node_room.type_of_room.center.y + shift_y);
            int x = (int)(node_room.center_on_map.x + node_room.type_of_room.doors.array[i].x - node_room.type_of_room.center.x + shift_x);
            map[y][x] = '0' + node_room_index;
        }
        //map[(int)(node_room.center_on_map.y + shift_y)][(int)(node_room.center_on_map.x + shift_x)] = '0' + node_room_index;
    }
    for (int i = 0; i < graph->n_nodes; i++) {
        if (graph->is_exist[i])
            for (int j = 0; j < graph->array_of_rooms[i]->len_nexts; j++) {
                map = build_corridor(graph, map, graph->array_of_rooms[i], graph->array_of_rooms[i]->next_nodes[j], shift_y, shift_x, *width, *height);
            }
    }
    for (int y = 0; y < *height; y++) {
        for (int x = 0; x < *width; x++) {
            if (map[y][x] == '.')
                map[y][x] = ' ';
            if (map[y][x] != ' ')// && map[y][x] < '0'&& map[y][x] > '0')
                map[y][x] = '#';
        }
    }
    player_pos->x = (int)(graph->start->center_on_map.y + shift_y + 0.5);
    player_pos->y = (int)(graph->start->center_on_map.x + shift_x + 0.5);
    return map;
}

void create_map(world_t* world) {
    graph_of_rooms_t graph = read_graph_from_file("graph.txt");
    int width, height;
    char** map = put_node_rooms_on_map_from_graph(&graph, &width, &height, &world->player.pos);
    for (int i = 0; i < height; i++) {
        fprintf(log_file(), "%s\n", map[i]);
    }
    for (int i = 0; i < world->door_array.len; i++) {
        map[(int)world->door_array.array[i].pos.x][(int)world->door_array.array[i].pos.y] = 'd';
    }
    world->map = map;
    world->map_height = height;
    world->map_width = width;
}
