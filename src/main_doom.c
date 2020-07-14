#include "sprite.h"
#include "olc/olc.h"
#include <stdio.h>

sprite_t test_sprite;
int width = 50;
int height = 30;
int glyph_size = 20;

int stop = 0;

int create() {

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
        save_sprite_to_file("sprite.spr", &test_sprite);
        return 0;
    }

    const char* hello_str = "Hello world";    
    olc_draw(0, 0, 'X', get_sprite_color(0, 0, &test_sprite));
    olc_draw(1, 0, 'X', get_sprite_color(1, 0, &test_sprite));
    olc_draw(2, 0, 'X', get_sprite_color(2, 0, &test_sprite));
    olc_draw(0, 1, 'X', get_sprite_color(0, 1, &test_sprite));
    olc_draw(1, 1, 'X', get_sprite_color(1, 1, &test_sprite));
    olc_draw(2, 1, 'X', get_sprite_color(2, 1, &test_sprite));
    olc_draw(0, 2, 'X', get_sprite_color(0, 2, &test_sprite));
    olc_draw(1, 2, 'X', get_sprite_color(1, 2, &test_sprite));
    olc_draw(2, 2, 'X', get_sprite_color(2, 2, &test_sprite));
    return 1;
}

int main() {
    init_sprite(3, 3, &test_sprite);
    load_sprite_from_file("sprite.spr", &test_sprite);
    if (olc_initialize(width, height, glyph_size, glyph_size) == 0) {
        fprintf(stderr, "Cannot initialize olc");
        return 0;
    }
    olc_register_create(&create);
    olc_register_update(&update);

    olc_start(); // block until update return 0
    olc_deinitialize();
    system("pause");

    return 0;
}

