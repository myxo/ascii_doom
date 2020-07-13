#define _USE_MATH_DEFINES

#include "lib.h"

#include "world_object.h"

#include "rendering.h"

#include <windows.h>

#include <stdio.h>

#include <math.h>


int width = 200;
int height = 150;
int glyph_size = 8;

int stop = 0;

world_t world;

int Wbutton = 0x57;
int Abutton = 0x41;
int Sbutton = 0x53;
int Dbutton = 0x44;

void move_player(int forward, int right, float time_elapsed) {
	double tmp_x = world.player_pos_x;
	tmp_x += forward * time_elapsed * world.player_speed * sin(world.player_angle);
	tmp_x += right * time_elapsed * world.player_speed * sin(world.player_angle + M_PI_2);
	if (map[(int)tmp_x][(int)world.player_pos_y] != '#')
		world.player_pos_x = tmp_x;
	double tmp_y = world.player_pos_y;
	tmp_y += forward * time_elapsed * world.player_speed * cos(world.player_angle);
	tmp_y += right * time_elapsed * world.player_speed * cos(world.player_angle + M_PI_2);
	if (map[(int)world.player_pos_x][(int)tmp_y] != '#')
		world.player_pos_y = tmp_y;
}

int create() {
	return 1;
}

void handle_input(float time_elapsed) {
	if (olc_get_key(VK_ESCAPE).held) { // Esc
		stop = 1;
	}
	if (olc_get_key(VK_LEFT).held) { // Esc
		world.player_angle -= 0.02;
	}
	if (olc_get_key(VK_RIGHT).held) { // Esc
		world.player_angle += 0.02;
	}
	if (olc_get_key(Wbutton).held) { // Esc
		move_player(1, 0, time_elapsed);
	}
	if (olc_get_key(Abutton).held) { // Esc
		move_player(0, -1, time_elapsed);
	}
	if (olc_get_key(Sbutton).held) { // Esc
		move_player(-1, 0, time_elapsed);
	}
	if (olc_get_key(Dbutton).held) { // Esc
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
	


	olc_start(); // block until update return 0
	olc_deinitialize();

	return 0;
}
