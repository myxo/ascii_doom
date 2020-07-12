#include "lib.h"

#include <stdio.h>

#include <stdlib.h>

#include <windows.h>

#include <time.h>


#define UP 1
#define DOWN -1
#define LEFT -2
#define RIGHT 2

int width = 50;
int height = 50;
int glyph_size = 16;
int TPS = 15;

int stop = 0;

typedef struct {
	double value;
	const char* label;
} var_to_display_t;

typedef struct {
	var_to_display_t* vars;
	int length;
}vars_to_display_t;
vars_to_display_t logging;

typedef struct {
	int x;
	int y;
} food_t;

typedef struct {
	int x;
	int y;
	int direction;
} node_t;

typedef struct {
	int length;
	node_t* body;
} snake_t;

typedef struct {
	food_t food;
	snake_t snake;
} world_t;

world_t world;

food_t food_init() {
	food_t* food = malloc(sizeof(food_t));
	food->x = rand() % width;
	food->y = rand() % height;
	return *food;
}

snake_t snake_init() {
	snake_t* snake = malloc(sizeof(snake_t));
	snake->body = malloc(sizeof(node_t));
	snake->body[0].x = 10;
	snake->body[0].y = 10;
	snake->length = 1;
	snake->body[0].direction = UP;
	return *snake;
}

void snake_move(snake_t* snake) {
	for (int i = 0; i < snake->length; i++) {
		switch (snake->body[i].direction) {
		case UP:
			if (--snake->body[i].y == -1) {
				snake->body[i].y = height;
			}
			break;
		case RIGHT:
			snake->body[i].x = (++snake->body[i].x) % width;
			break;
		case DOWN:
			snake->body[i].y = (++snake->body[i].y) % height;
			break;
		case LEFT:
			if (--snake->body[i].x == -1) {
				snake->body[i].x = width;
			}
			break;
		}
	}
	
	for (int i = snake->length - 1; i > 0; i--) {
		snake->body[i].direction = snake->body[i - 1].direction;
	}
}

void snake_expand(snake_t* snake) {
	node_t node;
	node.x = snake->body[snake->length - 1].x;
	node.y = snake->body[snake->length - 1].y;
	node.direction = snake->body[snake->length - 1].direction;
	switch (snake->body[snake->length - 1].direction) {
	case UP:
		node.y++;
		break;
	case RIGHT:
		node.x--;
		break;
	case DOWN:
		node.y--;
		break;
	case LEFT:
		node.x++;
		break;
	}
	snake->body = realloc(snake->body, ++snake->length * sizeof(node));
	snake->body[snake->length - 1] = node;
}

void snake_turn(snake_t* snake, int direction) {
	if (direction != -snake->body[0].direction || snake->length == 1)
		snake->body[0].direction = direction;
}

void snake_draw(snake_t snake) {
	for (int i = 0; i < snake.length; i++) {
		olc_draw(snake.body[i].x, snake.body[i].y, '#', FG_BLUE);
	}
}

int collision_detecter() {
	if (world.snake.body[0].x == world.food.x && world.snake.body[0].y == world.food.y) {
		//free(&world.food);
		world.food = food_init();
		snake_expand(&world.snake);
		return 1;
	}
	if (world.snake.length == 0) {
		return 1;
	}
	for (int i = 1; i < world.snake.length; i++) {
		if (world.snake.body[0].x == world.snake.body[i].x && world.snake.body[0].y == world.snake.body[i].y) {
			//free(&world.food);
			return 0;
		}
	}
	return 1;
}

//void add_watch(const char* label, double value) {
//	logging.vars = realloc(logging.vars, ++logging.length * sizeof(var_to_display_t));
//	logging.vars[logging.length - 1].label = label;
//	logging.vars[logging.length - 1].value = value;
//}
//
//int stop_watch(const char* label) {
//	int index = -1;
//	for (int i = 0; i < logging.length; i++) {
//		if (strcmp(logging.vars[i].label, label) == 0) {
//			index = i;
//			break;
//		}
//	}
//	if (index == -1) {
//		return 0;
//	}
//	for (int i = 0; i < logging.length - index; i++) {
//		logging.vars[index + i] = logging.vars[index + i + 1];
//	}
//	logging.vars = realloc(logging.vars, --logging.length * sizeof(var_to_display_t));
//}
//
//void display_watch() {
//	char* str;
//	int dec, sign;
//	for (int i = 0; i < logging.length; i++) {
//		str = "";
//		str = strcat(str, logging.vars[i].label);
//		str = strcat(str, " : ");
//		str = strcat(str, ecvt(logging.vars[i].value, 3, &dec, &sign));
//		olc_draw_string(0, i, str, FG_WHITE);
//	}
//}

int create() {
	return 1;
}

void handle_input(float time_elapsed) {
	if (olc_get_key(VK_ESCAPE).held) {
		stop = 1;
	}
	if (olc_get_key(VK_SPACE).held) {
		snake_expand(&world.snake);
	}
	if (olc_get_key(VK_UP).held) {
		snake_turn(&world.snake, UP);
	}
	if (olc_get_key(VK_RIGHT).held) {
		snake_turn(&world.snake, RIGHT);
	}
	if (olc_get_key(VK_DOWN).held) {
		snake_turn(&world.snake, DOWN);
	}
	if (olc_get_key(VK_LEFT).held) {
		snake_turn(&world.snake, LEFT);
	}
}

int update(float time_elapsed) {
	handle_input(time_elapsed);
	if (stop) {
		return 0;
	}
	//add_watch("head x", world.snake.body[0].x);
	//add_watch("head y", world.snake.body[0].y);
	olc_fill(0, 0, olc_screen_width(), olc_screen_height(), ' ', BG_BLACK);

	snake_move(&world.snake);

	snake_draw(world.snake);
	//display_watch();
	olc_draw(world.food.x, world.food.y, '*', FG_RED);

	//stop_watch("head x");
	//stop_watch("head y");

	Sleep(1000/TPS);
	//const char * hello_str = "Hello world";
	//olc_draw_string(width / 2 - strlen(hello_str) / 2, height / 2, hello_str, FG_WHITE);

	return collision_detecter();
}

int main() {
	srand(time(0));
	if (olc_initialize(width, height, glyph_size, glyph_size) == 0) {
		fprintf(stderr, "Cannot initialize olc");
		return 0;
	}
	olc_register_create(&create);
	olc_register_update(&update);
	world.food = food_init();
	world.snake = snake_init();
	logging.vars = calloc(1, sizeof(var_to_display_t));
	logging.length = 0;

	olc_start(); // block until update return 0
	olc_deinitialize();
	return 0;
}
