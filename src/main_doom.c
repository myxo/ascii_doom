#include "olc/olc.h"

#include "world_object.h"

#include "render.h"

#include <windows.h>

#include <stdio.h>

#include <math.h>


int width =  400;
int height =  250;
int glyph_size =  4;

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

void shoot_bullet(world_t* world) {
    if (world->bullet_array.len >= world->bullet_array.capacity)
        increase_arr_bullets_capacity(world->bullet_array);
    world->bullet_array.array[world->bullet_array.len].angle = world->player.angle;
    world->bullet_array.array[world->bullet_array.len].pos = world->player.pos;
    world->bullet_array.array[world->bullet_array.len].speed = 2;
    world->bullet_array.array[world->bullet_array.len].radius = 1;
    world->bullet_array.len++;
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
    if (olc_get_key(VK_SPACE).held) {
        shoot_bullet(get_world());
    }
}

void increase_arr_bullets_capacity(bullet_array_t bullet_array) {
    bullet_array.capacity = bullet_array.capacity * 2;
    bullet_array.array = realloc(bullet_array.array, bullet_array.capacity * sizeof(bullet_t));
}

void bullet_destruct(world_t* world, int index) {
    for (int i = index; i < world->bullet_array.len - 1; i++) {
        world->bullet_array.array[i] = world->bullet_array.array[i + 1];
    }
    world->bullet_array.len--;
}

void bullets_movement(world_t* world, float time_elapsed) {
    for (int i = 0; i < world->bullet_array.len; i++) {
        double new_x = world->bullet_array.array[i].pos.x;
        new_x += time_elapsed * world->bullet_array.array[i].speed * sin(world->bullet_array.array[i].angle);
        double new_y = world->bullet_array.array[i].pos.y;
        new_y += time_elapsed * world->bullet_array.array[i].speed * cos(world->bullet_array.array[i].angle);
        if (!is_wall(new_x, new_y)) {
            world->bullet_array.array[i].pos.x = new_x;
            world->bullet_array.array[i].pos.y = new_y;
        }
        else {
            bullet_destruct(get_world(), i);
        }
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

    for (int i = 0; i < get_world()->bullet_array.len; i++) {
        add_watch("bullet x", get_world()->bullet_array.array[i].pos.x);
        add_watch("bullet y", get_world()->bullet_array.array[i].pos.y);
        add_watch("0", 0);
    }
    //bullets_movement(getworld(), time_elapsed);
	draw_screen(get_world());
    display_watch();
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
