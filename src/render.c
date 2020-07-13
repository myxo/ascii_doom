#include "..\third_party\lib.h"

#include "world_object.h"

#include "render.h"

#include <math.h>

char map[16][16] = {
	{"###############"},
	{"# ##          #"},
	{"#             #"},
	{"#  ####       #"},
	{"#  #  #       #"},
	{"#  #  #       #"},
	{"#     #       #"},
	{"###############"}
};

void draw_screen(world_t world, int width, int height) {
	int threshold1 = 125;
	int threshold2 = 140;
	double d_angle = world.player_angle_of_vision / width;
	double ray_angle = world.player_angle - world.player_angle_of_vision / 2;
	double d_distance = 0.01;
	int row = 0;
	for (; ray_angle < world.player_angle + world.player_angle_of_vision / 2; ray_angle += d_angle) {
		double x = world.player_pos_x;
		double y = world.player_pos_y;
		double distance = 0;
		double ray_sin = sin(ray_angle);
		double ray_cos = cos(ray_angle);
		while (map[(int)x][(int)y] != '#') {
			x += d_distance * ray_sin;
			y += d_distance * ray_cos;
			distance += d_distance;
		}
		int num_of_wall_sym = height * (2 / (distance));
		int ceiling_level = (height - num_of_wall_sym) / 2;
		int floor_level = (height + num_of_wall_sym) / 2;
		if (num_of_wall_sym > height)
			num_of_wall_sym = height;
		char sym = '#';
		if (distance > 1.7)
			sym = '"';
		if (distance > 3)
			sym = '.';
		for (int i = ceiling_level; i < floor_level; i++) {
			olc_draw(row, i, sym, FG_WHITE);
		}
		for (int i = height / 2; i < height; i++) {
			if (i >= floor_level) {
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
		}
		row++;
	}
}