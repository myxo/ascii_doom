#include "olc/olc.h"

#include "sprite.h"
#include "world_object.h"
#include "render.h"
#include "config.h"

#define _USE_MATH_DEFINES
#include <math.h>


void draw_object(player_t* player, point_t obj_pos, double obj_radis, char ch, enum COLOR col, int obj_height) {
    double angle_from_player_to_obj = get_angle_from_pos1_to_pos2(player->pos, obj_pos);
    if (angle_from_player_to_obj < 0)
        angle_from_player_to_obj += 2 * M_PI;
    double x = player->pos.x;
    double y = player->pos.y;

    double start_player_view_angle = player->angle - player->angle_of_vision / 2;
    double stop_player_view_angle = player->angle + player->angle_of_vision / 2;
    if ((player->angle < start_player_view_angle) || (player->angle > stop_player_view_angle))
        return;
    double distance = get_distance_from_pos1_to_pos2(player->pos, obj_pos);
    double player_to_obj_width_angle = atan2(obj_radis, distance);
    double angle_from_player_to_obj_left = angle_from_player_to_obj - player_to_obj_width_angle;
    double angle_from_player_to_obj_right = angle_from_player_to_obj + player_to_obj_width_angle;

    double player_angle_floor_PI = player->angle - ((int)(player->angle / 2 / M_PI)) * 2 * M_PI;
    double start_player_view_angle_floor_PI = player_angle_floor_PI - player->angle_of_vision / 2;
    if (start_player_view_angle_floor_PI < 0)
        start_player_view_angle_floor_PI += 2 * M_PI;
    int row_left = olc_screen_width() * (angle_from_player_to_obj_left - start_player_view_angle_floor_PI) / player->angle_of_vision + 0.5;
    if (row_left < 0)
        row_left = 0;
    if (row_left > olc_screen_width())
        row_left = olc_screen_width();
    int row_right = olc_screen_width() * (angle_from_player_to_obj_right - start_player_view_angle_floor_PI) / player->angle_of_vision + 0.5;
    if (row_right > olc_screen_width())
        row_right = olc_screen_width();
    if (row_right < 0)
        row_right = 0;
    obj_height = obj_height / distance;
    int draw_start = olc_screen_height() / 2 - obj_height + 0.5;
    int draw_end = olc_screen_height() / 2 + obj_height + 0.5;
    if (draw_end > olc_screen_height())
        draw_end = olc_screen_height();
    if (draw_start < 0)
        draw_start = 0;
    for (int i = row_left; i <= row_right; i++)
        for (int j = draw_start; j < draw_end; j++) {
            if (distance < get_world()->z_buffer[i][j]) {
                olc_draw(i, j, ch, col);
                get_world()->z_buffer[i][j] = distance;
            }
        }
}

void draw_enemies(world_t* world) {
    player_t* player = &world->player;
    for (int i = 0; i < world->enemy_array.len; i++) {
        enemy_t* enemy = &world->enemy_array.array[i];
        draw_object(player, enemy->pos, enemy->radius, '%', FG_BLUE, 50);
    }
}

void draw_bullets(world_t* world) {
    player_t* player = &world->player;
    for (int i = 0; i < world->bullet_array.len; i++) {
        bullet_t* bullet = &world->bullet_array.array[i];
        draw_object(player, bullet->pos, bullet->radius, '*', FG_RED, 4);
    }
}

void draw_screen(world_t* world) {
    int width = olc_screen_width();
    int height = olc_screen_height();
    int threshold1 = 125;
    int threshold2 = 140;
    double d_angle = world->player.angle_of_vision / width;
    double ray_angle = world->player.angle - world->player.angle_of_vision / 2;
    double d_distance = get_config_value(kRayTraceStep);
    int row = 0;
    int bullet_row = 0;
    for (; ray_angle < world->player.angle + world->player.angle_of_vision / 2; ray_angle += d_angle) {
        double x = world->player.pos.x;
        double y = world->player.pos.y;
        double distance = 0;
        double ray_sin = sin(ray_angle);
        double ray_cos = cos(ray_angle);
        int bullet_height = 0;
        while (!is_wall(x, y)) {
            x += d_distance * ray_sin;
            y += d_distance * ray_cos;
            distance += d_distance;
        }
        int num_of_wall_sym = height * (1 / (distance));
        if (num_of_wall_sym > height)
            num_of_wall_sym = height;
        int ceiling_level = (height - num_of_wall_sym) / 2;
        int floor_level = (height + num_of_wall_sym) / 2;
        char sym = '#';
        double dx = fabs(x - round(x));
        double dy = fabs(y - round(y));
        double sprite_x;
        if (dy > dx) {
            sprite_x = fabs(y - (int)(y));
            if (is_wall(x - 0.2, y)) {
                sprite_x = 1 - sprite_x;
            }
        } else {
            sprite_x = fabs(x - (int)(x));
            if (is_wall(x, y + 0.2)) {
                sprite_x = 1 - sprite_x;
            }
        }
        for (int i = 0; i < ceiling_level; i++) {
            world->z_buffer[row][i] = MAX_BUFF;
        }
        for (int i = ceiling_level; i < floor_level; i++) {
            double sprite_y = (i - ceiling_level) / (double)num_of_wall_sym;
            olc_draw(row, i, sym, sample_sprite_color(sprite_x, sprite_y, world->textures.wall, 0));
            world->z_buffer[row][i] = distance;
        }
        for (int i = floor_level; i < height; i++) {
            if (i < threshold1) {
                olc_draw(row, i, '-', FG_GREY);
            }
            else if (i < threshold2) {
                olc_draw(row, i, 'x', FG_GREY);
            }
            else {
                olc_draw(row, i, 'X', FG_GREY);
            }
            world->z_buffer[row][i] = MAX_BUFF;
        }
        row++;
    }
    draw_enemies(world);
    draw_bullets(world);
}

void draw_minimap(world_t* world) {

    for (int i = 0; i < world->map_height; i++) {
        for (int j = 0; j < world->map_width; j++) {
            enum COLOR sym_col_BG;
            enum COLOR sym_col_FG;
            char sym = world->map[i][j];
            if (sym == '#') {
                sym_col_FG = FG_GREY;
                sym_col_BG = BG_GREY;
            }
            else {
                sym_col_FG = FG_BLACK;
                sym_col_BG = BG_BLACK;
            }
            olc_draw(i, world->map_width - j - 1, sym, sym_col_FG + sym_col_BG);
        }
    }
    double d_angle = world->player.angle_of_vision / olc_screen_width();
    double ray_angle = world->player.angle - world->player.angle_of_vision / 2;
    double d_distance = 0.1;
    for (; ray_angle < world->player.angle + world->player.angle_of_vision / 2; ray_angle += d_angle) {
        double x = world->player.pos.x;
        double y = world->player.pos.y;
        double ray_sin = sin(ray_angle);
        double ray_cos = cos(ray_angle);
        for (int i = 0; i < 2; i++) {
            double last_x = x;
            double last_y = y;
            while ((int)last_x == (int)x && (int)last_y == (int)y) {
                x += d_distance * ray_sin;
                y += d_distance * ray_cos;
            }
            if ((int)x >= 0 && (int)x < world->map_height && (int)y >= 0 && (int)y < world->map_width) {
                if (world->map[(int)x][(int)y] == '#') {
                    olc_draw((int)x, world->map_width - (int)y - 1, '*', FG_RED + BG_GREY);
                }
                else {
                    olc_draw((int)x, world->map_width - (int)y - 1, '*', FG_RED + BG_BLACK);
                }
            }
        }
    }
    olc_draw((int)world->player.pos.x, world->map_width - (int)world->player.pos.y - 1, '@', FG_GREEN);
    for (int i = 0; i < world->enemy_array.len; i++) {
        olc_draw((int)world->enemy_array.array[i].pos.x, world->map_width - (int)world->enemy_array.array[i].pos.y - 1, '%', FG_GREEN);
    }
}

void draw_sprite(sprite_t* sprite, int x, int y, double distance, int texture_index) {
    for (int i = 0; i < sprite->texture[texture_index].width; i++) {
        for (int j = 0; j < sprite->texture[texture_index].height; j++) {
            char sym = get_sprite_glyph(i, j, sprite, texture_index);
            if (sym != ' ' && distance < get_world()->z_buffer[i + x][j + y]) {
                olc_draw(i + x, j + y, sym, get_sprite_color(i, j, sprite, texture_index));
                get_world()->z_buffer[i + x][j + y] = distance;
            }
        }
    }
}
