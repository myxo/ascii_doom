#include "sprite.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <cassert>

void set_sprite_color(int x, int y, sprite_t* sprite, short color) {
    assert(x >= 0);
    assert(y >= 0);
    assert(x < sprite->width);
    assert(y < sprite->height);
    sprite->colours[(sprite->width * y) + x] = color;
}

void init_sprite(int width, int height, sprite_t* sprite) {
    sprite->width = width;
    sprite->height = height;
    sprite->colours = malloc(width * height, sizeof(short));
    for (int i = 0; i < width * height; i++) {
        sprite->colours[i] = FG_WHITE;
    }
}
void deinit_sprite(sprite_t* sprite) {
    sprite->height = 0;
    sprite->width = 0;
    free(sprite->colours);
}

short get_sprite_color(int x, int y, sprite_t* sprite) {
    assert(x >= 0);
    assert(y >= 0);
    assert(x < sprite->width);
    assert(y < sprite->height);
    return sprite->colours[(sprite->width * y) + x];
}

void load_sprite_from_file(const char* filename, sprite_t* sprite) {
    FILE* file = fopen(filename, "rb");
    int size[2];
    fread(size, sizeof(int), 2, file);
    sprite->width = size[0];
    sprite->height = size[1];
    int len = sprite->width * sprite->height;
    sprite->colours = realloc(sprite->colours, len * sizeof(short));
    fread(sprite->colours, sizeof(short), len, file);
    fclose(file);
}

void save_sprite_to_file(const char* filename, sprite_t* sprite) {
    FILE* file = fopen(filename, "wb");
    int size[2];
    size[0] = sprite->width;
    size[1] = sprite->height;
    int len = sprite->width * sprite->height;
    fwrite(size, sizeof(int), 2, file);
    fwrite(sprite->colours, sizeof(int), len, file);
    fclose(file);
}

