#define _USE_MATH_DEFINES
#include "sprite.h"
#include "olc/olc.h"
#include "UI.h"

#include "world_object.h"

#include <stdlib.h>

#include <stdio.h>

#include <math.h>

#include <windows.h>
#include "bullet.h"

world_t* world_global = NULL;

void init_bullet_array(world_t* world, int capacity) {
    world_global->bullet_array.capacity = capacity;
    world_global->bullet_array.len = 0;
    world_global->bullet_array.array = malloc(world_global->bullet_array.capacity * sizeof(bullet_t));
}


void init_enemy_array(world_t* world, int capacity) {
    world_global->enemy_array.capacity = capacity;
    world_global->enemy_array.len = 0;
    world_global->enemy_array.array = malloc(world_global->enemy_array.capacity * sizeof(enemy_t));
}


int init_world_object() {
    world_global = malloc(sizeof(world_t));
    world_global->player.health = 3;
    world_global->player.pos.x = 1;
    world_global->player.pos.y = 1;
    world_global->player.angle = M_PI_4;
    world_global->player.speed = 2.5;
    world_global->player.angle_of_vision = M_PI_4;
    world_global->player.radius = 0.2;
    world_global->player.angular_speed = 1.2;
    world_global->player.time_from_last_shot = 0;
    world_global->stop = 0;
    init_bullet_array(world_global, 5);
    world_global->textures.wall = malloc(sizeof(sprite_t));
    init_sprite(1, 1, world_global->textures.wall);
    load_sprite_from_file("wall1.spr", world_global->textures.wall);
    init_enemy_array(world_global, 5);
    world_global->game_layouts.main_menu = init_canvas(update_main_menu);
    init_main_menu(world_global->game_layouts.main_menu);
    world_global->game_layouts.game = init_canvas(game_update);
    world_global->game_layouts.main_menu->is_active = 1;
    return read_map_for_file();
}

void deinit_world_object() {
    for (int i = 0; i < world_global->map_height; i++) {
        free(world_global->map[i]);
    }
    free(world_global->map);
    deinit_sprite(world_global->textures.wall);
    deinit_canvas(world_global->game_layouts.main_menu);
    deinit_canvas(world_global->game_layouts.game);
    free(world_global);
}

world_t* get_world() {
    return world_global;
}

int is_wall(double x, double y) {
    if ((int)x < 0 || (int)y < 0)
        return 1;
    if ((int)x >= get_world()->map_height || (int)y >= get_world()->map_width)
        return 1;
    return world_global->map[(int)x][(int)y] == '#';
}

int read_map_for_file() {
    FILE* fmap;
    fmap = fopen("map.txt", "r");
    if (fmap == NULL) {
        return 0;
    }
    char tmp;
    fscanf(fmap, "%d %d", &world_global->map_width, &world_global->map_height);
    world_global->map = malloc(world_global->map_height * sizeof(char*));
    for (int i = 0; i < world_global->map_height; i++) {
        world_global->map[i] = malloc((world_global->map_width + 1) * sizeof(char));
    }
    fgetc(fmap);
    for (int i = 0; i < world_global->map_height; i++) {
        for (int j = 0; j < world_global->map_width; j++) {
            world_global->map[i][j] = fgetc(fmap);
        }
        world_global->map[i][world_global->map_width] = 0;
        fgetc(fmap);
    }
    fclose(fmap);
    return 1;
}

int is_bullet(double x, double y) {
    for (int i = 0; i < world_global->bullet_array.len; i++) {
        double ox_vec = x - world_global->bullet_array.array[i].pos.x;
        double oy_vec = y - world_global->bullet_array.array[i].pos.y;
        double r = world_global->bullet_array.array[i].radius;
        if (pow(ox_vec, 2) + pow(oy_vec, 2) <= pow(r, 2)){
            return 1;
        }
    }
    return 0;
}

int is_in_circle(point_t pos, point_t circle_center, double radius) {
    double dx = pos.x - circle_center.x;
    double dy = pos.y - circle_center.y;
    return pow(dx, 2) + pow(dy, 2) <= pow(radius, 2);
}

int is_player(double x, double y) {
    point_t pos = {x , y};
    return is_in_circle(pos, world_global->player.pos, world_global->player.radius);
}

int is_enemy(double x, double y, int* enemy_index) {
    for (int i = 0; i < world_global->enemy_array.len; i++) {
        point_t pos = {x, y};
        if (is_in_circle(pos, world_global->enemy_array.array[i].pos, world_global->enemy_array.array[i].radius)) {
            if (enemy_index != NULL)
                *enemy_index = i;
            return 1;
        }
    }
    return 0;
}

point_t get_rand_pos_on_floor(world_t* world) {
    point_t pos;
    do {
        pos.x = rand() % world->map_width;
        pos.y = rand() % world->map_height;
    } while (is_wall(pos.x, pos.y));
    return pos;
}

double get_angle_from_pos1_to_pos2(point_t pos1, point_t pos2) {
    double delta_x = pos2.x - pos1.x;
    double delta_y = pos2.y - pos1.y;
    return atan2(delta_x, delta_y);
}

double get_distance_from_pos1_to_pos2(point_t pos1, point_t pos2) {
    return sqrt(pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2));
}

int has_wall_between(point_t pos1, point_t pos2) {
    double x = pos1.x;
    double y = pos1.y;
    double angle = get_angle_from_pos1_to_pos2(pos1, pos2);
    double d_distance = 0.01;
    while (!is_wall(x, y)) {
        x += d_distance * sin(angle);
        y += d_distance * cos(angle);
        point_t pos = { x, y };
        if (is_in_circle(pos, pos2, 0.1)) {
            return 0;
        }
    }
    return 1;
}

void handle_player_movement(float time_elapsed) {
    if (olc_get_key(VK_LEFT).held) {
        turn_player(-1, time_elapsed);
    }
    if (olc_get_key(VK_RIGHT).held) {
        turn_player(1, time_elapsed);
    }
    if (olc_get_key('W').held) {
        move_player(1, 0, time_elapsed);
    }
    if (olc_get_key('A').held) {
        move_player(0, -1, time_elapsed);
    }
    if (olc_get_key('S').held) {
        move_player(-1, 0, time_elapsed);
    }
    if (olc_get_key('D').held) {
        move_player(0, 1, time_elapsed);
    }
    get_world()->player.time_from_last_shot += time_elapsed;
    if (olc_get_key(VK_SPACE).pressed) {
        if (get_world()->player.time_from_last_shot >= 0.5) {
            get_world()->player.time_from_last_shot = 0;
            shoot_bullet(get_world(), get_world()->player.pos, get_world()->player.angle, time_elapsed, kBulletPlayer);
        }
    }
}

void handle_input_game(float time_elapsed) {
    if (olc_get_key(VK_ESCAPE).held) {
        get_world()->stop = 1;
    }
    handle_player_movement(time_elapsed);
}

int game_update(float time_elapsed) {
    handle_input_game(time_elapsed);
    if (get_world()->stop) {
        return 0;
    }
    olc_fill(0, 0, olc_screen_width(), olc_screen_height(), ' ', BG_BLACK);

    if (get_world()->enemy_array.len == 0) {
        add_enemy(get_world());
    }
    bullets_movement(get_world(), time_elapsed);
    enemy_movement(get_world(), time_elapsed);
    draw_screen(get_world());
    draw_minimap(get_world());
    display_watch();
    return 1;
}

void handle_menu_input(float time_elapsed) {
    if (olc_get_key(VK_ESCAPE).held) {
        get_world()->stop = 1;
    }
    if (olc_get_key(VK_SPACE).pressed) {
        get_active_button(get_world()->game_layouts.main_menu)->action();
    }
}

void init_main_menu(canvas_t* menu_canvas) {
    add_button(menu_canvas, olc_screen_width()/2 - 10, olc_screen_height() / 2 - 5 , 10, 5, "Play", action_to_game);
    menu_canvas->buttons[0].is_active = 1;
}

int update_main_menu(float time_elapsed) {
    handle_menu_input(time_elapsed);
    if (get_world()->stop == 1) {
        return 0;
    }
    //olc_fill(0, 0, olc_screen_width(), olc_screen_height(), ' ', BG_BLACK);
    draw_canvas(get_world()->game_layouts.main_menu);
    return 1;
}

int action_to_game() {
    get_world()->game_layouts.main_menu->is_active = 0;
    get_world()->game_layouts.game->is_active = 1;
    return 1;
}
