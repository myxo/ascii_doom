#include "olc/olc.h"

#include "sprite.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <cassert>

void set_texture_glyph(int x, int y, texture_t* texture, char glyph) {
    assert(x >= 0);
    assert(y >= 0);
    assert(x < texture->width);
    assert(y < texture->height);
    texture->glyphs[(texture->width * y) + x] = glyph;
}

void set_texture_color(int x, int y, texture_t* texture, short color) {
    assert(x >= 0);
    assert(y >= 0);
    assert(x < texture->width);
    assert(y < texture->height);
    texture->colours[(texture->width * y) + x] = color;
}

void init_texture(int width, int height, texture_t* texture) {
    texture->width = width;
    texture->height = height;
    texture->colours = malloc(width * height * sizeof(short));
    texture->glyphs = malloc(width * height * sizeof(short));
    for (int i = 0; i < width * height; i++) {
        texture->colours[i] = FG_WHITE | BG_BLACK;
    }
    for (int i = 0; i < width * height; i++) {
        texture->glyphs[i] = '\0';
    }
}
void deinit_texture(texture_t* texture) {
    texture->height = 0;
    texture->width = 0;
    free(texture->colours);
    free(texture->glyphs);
}

short get_texture_color(int x, int y, texture_t* texture) {
    assert(x >= 0);
    assert(y >= 0);
    assert(x < texture->width);
    assert(y < texture->height);
    return texture->colours[(texture->width * y) + x];
}

char get_texture_glyph(int x, int y, texture_t* texture) {
    assert(x >= 0);
    assert(y >= 0);
    assert(x < texture->width);
    assert(y < texture->height);
    return texture->glyphs[(texture->width * y) + x];
}

short sample_texture_color(double x, double y, texture_t* texture) {
    assert(x >= 0);
    assert(y >= 0);
    assert(y <= 1);
    assert(x <= 1);
    int sprite_x = (int)round(x * (double)(texture->width - 1));
    int sprite_y = (int)round(y * (double)(texture->height - 1));
    return get_texture_color(sprite_x, sprite_y, texture);
}

void load_texture_from_file(const char* filename, texture_t* texture) {
    FILE* file = fopen(filename, "rb");
    int size[2];
    fread(size, sizeof(int), 2, file);
    texture->width = size[0];
    texture->height = size[1];
    int len = texture->width * texture->height;
    texture->colours = malloc(len * sizeof(short));
    fread(texture->colours, sizeof(short), len, file);
    texture->glyphs = malloc(len * sizeof(char));
    fread(texture->glyphs, sizeof(char), len, file);
    fclose(file);
}

void save_texture_to_file(const char* filename, texture_t* texture) {
    FILE* file = fopen(filename, "wb");
    int size[2];
    size[0] = texture->width;
    size[1] = texture->height;
    int len = texture->width * texture->height;
    fwrite(size, sizeof(int), 2, file);
    fwrite(texture->colours, sizeof(short), len, file);
    fwrite(texture->glyphs, sizeof(char), len, file);
    fclose(file);
}

void init_sprite(sprite_t* sprite) {
    sprite->capacity = 1;
    sprite->texture_count = 0;
    sprite->texture = calloc(sizeof(texture_t), sprite->capacity);
}

void deinit_sprite(sprite_t* sprite) {
    free(sprite->texture);
    sprite->capacity = 0;
    sprite->texture_count = 0;
}

void attach_texture_to_sprite(sprite_t* sprite, texture_t texture) {
    if (sprite->texture_count >= sprite->capacity) {
        sprite->capacity *= 2;
        sprite->texture = realloc(sprite->texture, sizeof(texture_t) * sprite->capacity);
    }
    sprite->texture[sprite->texture_count] = texture;
    sprite->texture_count++;
}

short get_sprite_color(int x, int y, sprite_t* sprite, int texture_index) {
    assert(texture_index < sprite->texture_count);
    return get_texture_color(x, y, &sprite->texture[texture_index]);
}
short sample_sprite_color(double x, double y, sprite_t* sprite, int texture_index) {
    assert(texture_index < sprite->texture_count);
    return sample_texture_color(x, y, &sprite->texture[texture_index]);
}
void set_sprite_color(int x, int y, sprite_t* sprite, short color, int texture_index) {
    assert(texture_index < sprite->texture_count);
    set_texture_color(x, y, &sprite->texture[texture_index], color);
}
char get_sprite_glyph(int x, int y, sprite_t* sprite, int texture_index) {
    assert(texture_index < sprite->texture_count);
    return get_texture_glyph(x, y, &sprite->texture[texture_index]);
}
void set_sprite_glyph(int x, int y, sprite_t* sprite, char glyph, int texture_index) {
    assert(texture_index < sprite->texture_count);
    set_texture_glyph(x, y, &sprite->texture[texture_index], glyph);
}
