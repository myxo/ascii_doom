#include "olc/olc.h"

#include "world_object.h"

#include "render.h"

#include <windows.h>

#include <stdio.h>

#include <math.h>


int width =  200;
int height =  150;
int glyph_size =  8;

int stop = 0;

void move_player(int forward, int right, float time_elapsed) {
    world_t* world = get_world();

	double new_x = world->player.pos.x;
	new_x += forward * time_elapsed * world->player.speed * sin(world->player.angle);
	new_x += right * time_elapsed * world->player.speed * cos(world->player.angle);
	if (!is_wall(new_x, world->player.pos.y))
		world->player.pos.x = new_x;

	double new_y = world->player.pos.y;
	new_y += forward * time_elapsed * world->player.speed * cos(world->player.angle);
	new_y -= right * time_elapsed * world->player.speed * sin(world->player.angle);
	if (!is_wall(world->player.pos.x, new_y))
		world->player.pos.y = new_y;
}

void turn_player(int dir) {
    world_t* world = get_world();
    world->player.angle += dir * world->player.angular_speed;
}

int create() {
    init_world_object();
    log_init("debug.txt");
	return 1;
}

void handle_player_movement(float time_elapsed) {
    if (olc_get_key(VK_LEFT).held) {
        turn_player(-1);
    }
    if (olc_get_key(VK_RIGHT).held) {
        turn_player(1);
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

void handle_input(float time_elapsed) {
    if (olc_get_key(VK_ESCAPE).held) {
        stop = 1;
    }
    handle_player_movement(time_elapsed);
}

int update(float time_elapsed) {
	handle_input(time_elapsed);
	if (stop) {
		return 0;
	}
	olc_fill(0, 0, width, height, ' ', BG_BLACK);
    display_watch();
	draw_screen(get_world());
	return 1;
}

int main() {
	if (olc_initialize(width, height, glyph_size, glyph_size) == 0) {
		fprintf(stderr, "Cannot initialize olc");
		return 0;
	}
	olc_register_create(&create);
	olc_register_update(&update);

	olc_start(); // block until update return 0
	olc_deinitialize();
    log_deinit();
    deinit_world_object();

	return 0;
}
