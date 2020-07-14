#include "olc/olc.h"

#include <string.h>
#include <stdio.h>
#include "config.h"
#include "world_object.h"


int width = 20;
int height = 15;
int glyph_size = 8;
int stop = 0;
const char str[50] = "C:/Users/lev/Documents/ascii_doom/cfg.txt";
int create() {
    read_config_from_file(str);
    init_world_object();
	return 1;
}

void handle_input(float time_elapsed) {
	if (olc_get_key(0x1B).held) { // Esc
		stop = 1;
	}
}

int update(float time_elapsed) {
	handle_input(time_elapsed);
	if (stop) {
		return 0;
	}

    const char* hello_str = "Hello world";
	olc_draw_string(width / 2 - strlen(hello_str) / 2, height / 2, hello_str, FG_WHITE);

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
    deinit_world_object();

	return 0;
}
