#define _USE_MATH_DEFINES

#include "lib.h"

#include "world_object.h"

#include "render.h"

#include <windows.h>

#include <stdio.h>

#include <math.h>


int width = 200;
int height = 150;
int glyph_size = 8;

int stop = 0;

world_t world;

void move_player(int forward, int right, float time_elapsed) {
	double new_x = world.player_pos_x;
	new_x += forward * time_elapsed * world.player_speed * sin(world.player_angle);
	new_x += right * time_elapsed * world.player_speed * cos(world.player_angle);
	if (map[(int)new_x][(int)world.player_pos_y] != '#')
		world.player_pos_x = new_x;
	double new_y = world.player_pos_y;
	new_y += forward * time_elapsed * world.player_speed * cos(world.player_angle);
	new_y -= right * time_elapsed * world.player_speed * sin(world.player_angle);
	if (map[(int)world.player_pos_x][(int)new_y] != '#')
		world.player_pos_y = new_y;
}

int create() {
	return 1;
}

void handle_input(float time_elapsed) {
	if (olc_get_key(VK_ESCAPE).held) {
		stop = 1;
	}
	if (olc_get_key(VK_LEFT).held) {
		world.player_angle -= world.player_angular_speed;
	}
	if (olc_get_key(VK_RIGHT).held) {
		world.player_angle += world.player_angular_speed;
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
}

int update(float time_elapsed) {
	handle_input(time_elapsed);
	if (stop) {
		return 0;
	}
	olc_fill(0, 0, width, height, ' ', BG_BLACK);

	draw_screen(world, width, height);

	return 1;
}

int main() {
	if (olc_initialize(width, height, glyph_size, glyph_size) == 0) {
		fprintf(stderr, "Cannot initialize olc");
		return 0;
	}
	olc_register_create(&create);
	olc_register_update(&update);

	world.player_pos_x = 2;
	world.player_pos_y = 6;
	world.player_angle = 0;
	world.player_speed = 1.5;
	world.player_angle_of_vision= M_PI_4;
	world.player_angular_speed = 0.02;


	olc_start(); // block until update return 0
	olc_deinitialize();

	return 0;
}
