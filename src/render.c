#include "../third_party/olc/olc.h"

#include "world_object.h"

#include "render.h"

#include <math.h>

void draw_screen(world_t* world) {
    int width = olc_screen_width();
    int height = olc_screen_height();
	int threshold1 = 125;
	int threshold2 = 140;
	double d_angle = world->player.angle_of_vision / width;
	double ray_angle = world->player.angle - world->player.angle_of_vision / 2;
	double d_distance = 0.01;
	int row = 0;
	for (; ray_angle < world->player.angle + world->player.angle_of_vision / 2; ray_angle += d_angle) {
		double x = world->player.pos.x;
		double y = world->player.pos.y;
		double distance = 0;
		double ray_sin = sin(ray_angle);
		double ray_cos = cos(ray_angle);
		while (!is_wall(x, y)) {
			x += d_distance * ray_sin;
			y += d_distance * ray_cos;
			distance += d_distance;
            world->seeable[(int)x][(int)y] = 1;
		}
        world->seeable[(int)x][(int)y] = 1;
		int num_of_wall_sym = height * (2 / (distance));
		int ceiling_level = (height - num_of_wall_sym) / 2;
		int floor_level = (height + num_of_wall_sym) / 2;
		if (num_of_wall_sym > height)
			num_of_wall_sym = height;
		char sym = '#';
		for (int i = ceiling_level; i < floor_level; i++) {
			olc_draw(row, i, sym, FG_WHITE);
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
		}
		row++;
	}
}

void draw_minimap(world_t* world) {

    for (int i = 0; i < world->map_height; i++) {
        for (int j = 0; j < world->map_width; j++) {
            enum COLOR sym_col_BG;
            enum COLOR sym_col_FG;
            char sym = world->map[j][i];
            if (sym == '#') {
                sym_col_FG = FG_GREY;
                sym_col_BG = BG_GREY;
            }
            else {
                sym_col_FG = FG_BLACK;
                sym_col_BG = BG_BLACK;
            }
            if (world->seeable[j][i]) {
                sym_col_FG = FG_DARK_GREY;
                //sym_col_BG = BG_WHITE;
                //sym_col_FG = FG_WHITE;
                sym = '*';
                world->seeable[j][i] = 0;
            }
            olc_draw(i, j, sym, sym_col_FG + sym_col_BG);
        }
    }
    olc_draw((int)world->player.pos.y, (int)world->player.pos.x, '@', FG_GREEN);
}
