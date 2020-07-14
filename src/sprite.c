#include "sprite.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


void* CheckNull(void* ptr)
{
    if (ptr == 0)
    {
        exit(1);
    }
    return ptr;
}

void init_sprite(int width, int height, sprite_t* sprite)
{
    sprite_t sprite2;
    sprite->width = width;
    sprite->height = height;
    sprite->colours = CheckNull(calloc(width * height, sizeof(short)));
    for (int i = 0; i < width * height; i++)
    {
        sprite->colours[i] = FG_BLACK;
    }
    return;
}
void deinit_sprite(sprite_t* sprite)
{
    sprite->height = 0;
    sprite->width = 0;
    free(sprite->colours);
    return;
}

short get_sprite_color(int x, int y, sprite_t* sprite) {
    return sprite->colours[(sprite->width * y) + x];
}
void set_sprite_color(int x, int y, sprite_t* sprite, short color) {
    sprite->colours[(sprite->width * y) + x] = color;
}

int load_sprite_from_file(const char* filename, sprite_t* sprite) {
    FILE* file;
    file = fopen(filename, "rb");
    int size[2];
    fread(size, sizeof(int), 2, file);
    sprite->width = size[0];
    sprite->height = size[1];
    int len = sprite->width * sprite->height;
    sprite->colours = CheckNull(calloc(len, sizeof(short)));
    fread(&sprite->colours, sizeof(short), len, file);
    fclose(file);
    return 0;
}

int save_sprite_to_file(const char* filename, sprite_t* sprite) {
    FILE* file;
    file = fopen(filename, "wb");
    int size[2];
    size[0] = sprite->width;
    size[1] = sprite->height;
    int len = sprite->width * sprite->height;
    fwrite(size, sizeof(int), 2, file);
    fwrite(sprite->colours, sizeof(int), len, file);
    fclose(file);
    return 0;
}

