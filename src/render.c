#include "olc/olc.h"

#include "sprite.h"
#include "world_object.h"
#include "render.h"
#include "config.h"
#include "logging.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

// ensure angle in interval [0; 2*M_PI)
double normilize_angle(double angle) {
    double pi2 = 2 * M_PI;
    while (angle < 0) { angle += pi2; }
    while (angle >= pi2) { angle -= pi2; }
    return angle;
}

screen_obj_t get_object_on_screen(player_t* player, point_t obj_pos, double obj_radis, double obj_height) {
    double angle_from_player_to_obj = normilize_angle(get_angle_from_pos1_to_pos2(player->pos, obj_pos));
    double player_angle = normilize_angle(player->angle);

    double distance = get_distance_from_pos1_to_pos2(player->pos, obj_pos);
    double obj_width_angle = atan2(obj_radis, distance);
    double angle_to_obj_left = angle_from_player_to_obj - obj_width_angle;
    double angle_to_obj_right = angle_from_player_to_obj + obj_width_angle;

    double start_view_angle = player_angle - player->angle_of_vision / 2;
    int lrow_left = (int)(olc_screen_width() * (angle_to_obj_left - start_view_angle) / player->angle_of_vision + 0.5);
    int lrow_right = (int)(olc_screen_width() * (angle_to_obj_right - start_view_angle) / player->angle_of_vision + 0.5);

    obj_height = (obj_height / distance);
    int lline_start = (int)(olc_screen_height() / 2 - obj_height + 0.5);
    int lline_end = (int)(olc_screen_height() / 2 + obj_height + 0.5);

    screen_obj_t res;
    res.row_left = lrow_left;
    res.row_right = lrow_right;
    res.line_start = lline_start;
    res.line_end = lline_end;
    res.distance = distance;
    return res;
}

void draw_object(player_t* player, point_t obj_pos, double obj_radis, char ch, enum COLOR col, int obj_height) {
    screen_obj_t obj = get_object_on_screen(player, obj_pos, obj_radis, obj_height);
    for (int i = obj.row_left; i <= obj.row_right; i++)
        for (int j = obj.line_start; j < obj.line_end; j++) {
            if (obj.distance < get_world()->z_buffer[i][j]) {
                olc_draw(i, j, ch, col);
                get_world()->z_buffer[i][j] = obj.distance;
            }
        }
}

void draw_enemies(world_t* world) {
    player_t* player = &world->player;
    for (int i = 0; i < world->enemy_array.len; i++) {
        enemy_t* enemy = &world->enemy_array.array[i];
        draw_sprite(world->sprites.mob1, 0, enemy->pos, enemy->radius, 40);
    }
}

void draw_bullets(world_t* world) {
    player_t* player = &world->player;
    for (int i = 0; i < world->bullet_array.len; i++) {
        bullet_t* bullet = &world->bullet_array.array[i];
        draw_sprite(world->sprites.bullet, 0, bullet->pos, bullet->radius, 150 * bullet->radius);
    }
}

void draw_rockets(world_t* world) {
    player_t* player = &world->player;
    for (int i = 0; i < world->rocket_array.len; i++) {
        rocket_t* rocket = &world->rocket_array.array[i];
        draw_object(player, rocket->pos, rocket->radius, '*', FG_RED, 4);
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
    draw_rockets(world);
    draw_explosions(world);
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
    player_t player = get_world()->player;
    screen_obj_t obj = get_object_on_screen(&player, pos, obj_radis, obj_height);
    if ((obj.line_end - obj.line_start) != 0 && (obj.row_right - obj.row_left) != 0) {
        for (int i = obj.row_left; i <= obj.row_right; i++) {
            if (i >= 0 && i <= olc_screen_width()) {
                double i_d = (double)(i - obj.row_left) / (obj.row_right - obj.row_left);
                for (int j = obj.line_start; j <= obj.line_end; j++) {
                    if (j >= 0 && j <= olc_screen_height()) {
                        double j_d = (double)(j - obj.line_start) / (obj.line_end - obj.line_start);
                        char sym = sample_sprite_glyph(i_d, j_d, sprite, texture_index);
                        if (sym != 0 && obj.distance < get_world()->z_buffer[i][j]) {
                            olc_draw(i, j, sym, sample_sprite_color(i_d, j_d, sprite, texture_index));
                            get_world()->z_buffer[i][j] = obj.distance;
                        }
                    }
                }
            }
        }
    }
}

void draw_explosions(world_t* world) {
    for (int i = 0; i < world->explosion_array.len; i++) {
        draw_explosion(world, world->explosion_array.array[i].pos, world->explosion_array.array[i].radius, world->explosion_array.array[i].life_time);
    }
}

void draw_explosion(world_t* world, point_t pos, double radius, double life_time) {
    screen_obj_t expl = get_object_on_screen(&get_world()->player, pos, 1, 1);
    point_t expl_center;
    expl_center.x = (expl.row_left + expl.row_right) / 2;
    expl_center.y = (expl.line_start + expl.line_end) / 2;
    radius /= expl.distance;
    radius *= (1 + 250 * life_time);
    int row_start = (int)(expl_center.x - radius / 2);
    int row_end = (int)(expl_center.x + radius / 2);
    int line_start = (int)(expl_center.y - radius / 2);
    int line_end = (int)(expl_center.y + radius / 2);
    for (int i = row_start; i < row_end; i++) {
        for (int j = line_start; j < line_end; j++) {
            int state = rand() % 2;
            if (i >= 0 && j >= 0 && i < olc_screen_width() && j < olc_screen_height() && state == 1 && expl.distance < get_world()->z_buffer[i][j]) {
                olc_draw(i, j, '*', FG_RED);
                get_world()->z_buffer[i][j] = expl.distance;
            }
        }
    }
}
