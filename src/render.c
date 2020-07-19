#include "olc/olc.h"

#include "sprite.h"
#include "world_object.h"
#include "render.h"
#include "config.h"
#include "logging.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

double get_object_on_screen(player_t* player, point_t obj_pos, double obj_radis, double obj_height, int* row_left, int* row_right, int* line_start, int* line_end) {
    double angle_from_player_to_obj = get_angle_from_pos1_to_pos2(player->pos, obj_pos);
    if (angle_from_player_to_obj < 0)
        angle_from_player_to_obj += 2 * M_PI;
    double x = player->pos.x;
    double y = player->pos.y;

    double start_player_view_angle = player->angle - player->angle_of_vision / 2;
    double stop_player_view_angle = player->angle + player->angle_of_vision / 2;
    if ((player->angle < start_player_view_angle) || (player->angle > stop_player_view_angle))
        return 0.0;
    double distance = get_distance_from_pos1_to_pos2(player->pos, obj_pos);
    double player_to_obj_width_angle = atan2(obj_radis, distance);
    double angle_from_player_to_obj_left = angle_from_player_to_obj - player_to_obj_width_angle;
    double angle_from_player_to_obj_right = angle_from_player_to_obj + player_to_obj_width_angle;

    double player_angle_floor_PI = player->angle - ((int)(player->angle / 2 / M_PI)) * 2 * M_PI;
    double start_player_view_angle_floor_PI = player_angle_floor_PI - player->angle_of_vision / 2;
    if (start_player_view_angle_floor_PI < 0)
        start_player_view_angle_floor_PI += 2 * M_PI;
    int lrow_left = ((int)(olc_screen_width() * (angle_from_player_to_obj_left - start_player_view_angle_floor_PI) / player->angle_of_vision + 0.5));
    if (lrow_left < 0)
        lrow_left = 0;
    if (lrow_left > olc_screen_width())
        lrow_left = olc_screen_width();
    int lrow_right = (int)(olc_screen_width() * (angle_from_player_to_obj_right - start_player_view_angle_floor_PI) / player->angle_of_vision + 0.5);
    if (lrow_right > olc_screen_width())
        lrow_right = olc_screen_width();
    if (lrow_right < 0)
        lrow_right = 0;
    obj_height = (obj_height / distance);
    int lline_start = (int)(olc_screen_height() / 2 - obj_height + 0.5);
    int lline_end = (int)(olc_screen_height() / 2 + obj_height + 0.5);
    if (lline_end > olc_screen_height())
        lline_end = olc_screen_height();
    if (lline_start < 0)
        lline_start = 0;
    *row_left = lrow_left;
    *row_right = lrow_right;
    *line_start = lline_start;
    *line_end = lline_end;
    return distance;
}

void draw_object(player_t* player, point_t obj_pos, double obj_radis, char ch, enum COLOR col, int obj_height) {
    int row_left = 0;
    int row_right = 0;
    int draw_start = 0;
    int draw_end = 0;
    double distance = 0;/* get_object_on_screen(player, obj_pos, obj_radis, obj_height, &row_left, &row_right, &draw_start, &draw_end);*/
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
    int tex_width = world->sprites.bullet->texture[0].width;
    int tex_height = world->sprites.bullet->texture[0].height;
    for (int i = 0; i < world->bullet_array.len; i++) {
        bullet_t* bullet = &world->bullet_array.array[i];
        draw_sprite(world->sprites.bullet, 0, bullet->pos, bullet->radius, 150 * bullet->radius);
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
        int num_of_wall_sym = (int)(height * (1 / (distance)));
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
            olc_draw(row, i, sym, sample_sprite_color(sprite_x, sprite_y, world->sprites.wall, 0));
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

void draw_hp(world_t* world) {
    int height = olc_screen_height() / 8;
    int width = olc_screen_width() / 3;
    world->player.health;
    double hp1 = (world->player.health * width) / world->player.maxhealth;
    olc_fill(0, olc_screen_height() - height, width, olc_screen_height(), ' ', BG_RED);
    olc_fill(0, olc_screen_height() - height, (int)round(hp1), olc_screen_height(), ' ', BG_GREEN + FG_WHITE);
}

void draw_sprite(sprite_t* sprite, int texture_index, point_t pos, double obj_radis, double obj_height) {
    int row_left = 0;
    int row_right = 0;
    int draw_start = 0;
    int draw_end = 0;
    player_t player = get_world()->player;
    double distance = get_object_on_screen(&player, pos, obj_radis, obj_height, &row_left, &row_right, &draw_start, &draw_end);
    if ((draw_end - draw_start) != 0 && (row_right - row_left) != 0) {
        for (int i = row_left; i <= row_right; i++) {
            double i_d = (double)(i - row_left) / (row_right - row_left);
            for (int j = draw_start; j <= draw_end; j++) {
                double j_d = (double)(j - draw_start) / (draw_end - draw_start);
                char sym = sample_sprite_glyph(i_d, j_d, sprite, texture_index);
                if (sym != 0 && distance < get_world()->z_buffer[i][j]) {
                    olc_draw(i, j, sym, sample_sprite_color(i_d, j_d, sprite, texture_index));
                    get_world()->z_buffer[i][j] = distance;
                }
            }
        }
    }
}
