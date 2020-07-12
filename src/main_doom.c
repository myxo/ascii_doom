#include "lib.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include<time.h> 


typedef struct point {
	int x;
	int y;
} point_t;

typedef struct array_point {
	point_t* array;
	int len;
	int capacity;
} array_point_t;

typedef struct player {
	array_point_t body;
	int score;
} player_t;

typedef struct sweet {
	point_t pos;
	int weight;
} sweet_t;

typedef struct world {
	player_t player;
	sweet_t* sweet;
} world_t;

world_t world;

int width = 50;
int height = 50;
int glyph_size = 12;

int stop = 0;
float time_count = 0;
int move_direction = VK_RIGHT;


array_point_t create_array_point(int capacity) {
	array_point_t array_point;
	array_point.array = malloc(capacity * sizeof(point_t));
	array_point.len = 0;
	array_point.capacity = capacity;
	return array_point;
}

void increase_arr_point_capacity(array_point_t* array_point) {
	array_point->capacity = array_point->capacity * 2;
	point_t* ptr = realloc(array_point->array, array_point->capacity * sizeof(point_t));
	array_point->array = ptr;
}

void push_back_array_point(array_point_t* array_point, point_t point) {
	if (array_point->len >= array_point->capacity) {
		increase_arr_point_capacity(array_point);
	}
	array_point->array[array_point->len++] = point;
}

sweet_t* generate_new_sweet(int x, int y, int width, int height) {
	sweet_t* sweet = malloc(sizeof(sweet_t));
	sweet->pos.x = (rand() % (width - x)) + x;
	sweet->pos.y = (rand() % (height - y)) + y;
	sweet->weight = 1;
	if (check_collision_player_sweet(world.player, *sweet ))
		return generate_new_sweet(x, y, width, height);
	return sweet;
}

void render_player(player_t player) {
	for (int i = 0; i < player.body.len; i++) {\
		olc_draw(player.body.array[i].x, player.body.array[i].y, '#', FG_WHITE);
	}
}

void render_sweet(sweet_t* sweet) {
	if (sweet == NULL)
		return;
	olc_draw(sweet->pos.x, sweet->pos.y, '*', FG_GREEN);
}

void render_world(world_t world) {
	olc_fill(0, 0, olc_screen_width(), olc_screen_height(), ' ', BG_BLACK);
	render_sweet(world.sweet);
	render_player(world.player);
}

player_t create_player(point_t pos, int len) {
	player_t player;
	player.body = create_array_point(len);
	for (int i = 0; i < len; i++) {
		push_back_array_point(&player.body, pos);
		pos.x--;
	}
	player.score = len;
	return player;
}

void player_move(player_t* player, int direction) {
	int x = player->body.array[0].x;
	int y = player->body.array[0].y;
	switch (direction)
	{
	case VK_UP:
		y++;
		break;
	case VK_DOWN:
		y--;
		break;
	case VK_LEFT:
		x--;
		break;
	case VK_RIGHT:
		x++;
		break;
	}
	for (int i = player->body.len; i >= 1; i--) {
		player->body.array[i].x = player->body.array[i-1].x;
		player->body.array[i].y = player->body.array[i-1].y;
	}
	player->body.array[0].x = x;
	player->body.array[0].y = y;
}

int create() {
	point_t start;
	start.x = width / 2;
	start.y = height / 2;
	world.player = create_player(start, 3);
	world.sweet = NULL;
	return 1;
}

int check_collision_player_sweet(player_t player, sweet_t sweet) {
	for (int i = 0; i < player.body.len; i++) {
		if (player.body.array[i].x == sweet.pos.x && player.body.array[i].y == sweet.pos.y)
			return 1;
	}
	return 0;
}

int check_collision_head_player(player_t player) {
	if (player.body.len < 2)
		return 0;
	for (int i = 2; i < player.body.len; i++) {
		if (player.body.array[i].x == player.body.array[0].x && player.body.array[i].y == player.body.array[0].y)
			return 1;
	}
	return 0;
}

int check_collision_walls_player(player_t player) {
	for (int i = 2; i < player.body.len; i++) {
		if (player.body.array[0].x < 0 || player.body.array[0].y < 0 || player.body.array[0].x == width || player.body.array[0].y == height)
			return 1;
	}
	return 0;
}

void handle_input(float time_elapsed) {
	time_count += time_elapsed;
	if (time_count >= 0.1) {
		if (olc_get_key(VK_ESCAPE).held) {
			stop = 1;
		}
		if (olc_get_key(VK_LEFT).held) {
			move_direction = VK_LEFT;
		}
		if (olc_get_key(VK_RIGHT).held) {
			move_direction = VK_RIGHT;
		}
		if (olc_get_key(VK_UP).held) {
			move_direction = VK_DOWN;
		}
		if (olc_get_key(VK_DOWN).held) {
			move_direction = VK_UP;
		}
		player_move(&world.player, move_direction);
		time_count = 0;
	}
}

int update(float time_elapsed) {
	handle_input(time_elapsed);
	if (stop) {
		return 0;
	}
	if (world.sweet != NULL) {
		if (check_collision_player_sweet(world.player, *(world.sweet))) {
			world.player.score = world.player.score + (*(world.sweet)).weight;
			free(world.sweet);
			world.sweet = NULL;
			push_back_array_point(&world.player.body, world.player.body.array[world.player.body.len - 1]);
		}
	}
	else {
		world.sweet = generate_new_sweet(0, 0, olc_screen_width(), olc_screen_height());
	}
	if (check_collision_head_player(world.player) || check_collision_walls_player(world.player))
		return 0;
	render_world(world);
	return 1;
}

int main() {
	//
	srand(time(0));
	if (olc_initialize(width, height, glyph_size, glyph_size) == 0) {
		fprintf(stderr, "Cannot initialize olc");
		return 0;
	}
	olc_register_create(&create);
	olc_register_update(&update);

	olc_start(); // block until update return 0
	olc_deinitialize();

	return 0;
}
