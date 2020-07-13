#include "..\third_party\lib.h"

#include "world_object.h"

#include "rendering.h"

#include <math.h>

char map[8][8] = {
	{"########"},
	{"# ##   #"},
	{"#      #"},
	{"#  #   #"},
	{"#  #   #"},
	{"#  #   #"},
	{"#  #   #"},
	{"########"}
};

void draw_screen(world_t world, int width, int height) {
	int threshold1 = 125;
	int threshold2 = 140;
	double d_angle = world.player_angle_of_vision / width;
	double ray_angle = world.player_angle - world.player_angle_of_vision / 2;
	double x = world.player_pos_x;
	double y = world.player_pos_y;
	double distance = 0;
	double d_distance = 0.01;
	int row = 0;
	for (; ray_angle < world.player_angle + world.player_angle_of_vision / 2; ray_angle += d_angle) {
		x = world.player_pos_x;
		y = world.player_pos_y;
		distance = 0;
		while (map[(int)x][(int)y] != '#') {
			x += d_distance * sin(ray_angle);
			y += d_distance * cos(ray_angle);
			distance += d_distance;
		}
		int num_of_wall_sym = height * (2 / (distance));
		if (num_of_wall_sym > height)
			num_of_wall_sym = height;
		char sym = '#';
		if (distance > 1.7)
			sym = '"';
		if (distance > 3)
			sym = '.';
		for (int i = 0; i < num_of_wall_sym; i++) {
			olc_draw(row, (height - num_of_wall_sym) / 2 + i, sym, FG_WHITE);
		}
		for (int i = height / 2; i < height; i++) {
			if (i >= (height + num_of_wall_sym) / 2) {
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