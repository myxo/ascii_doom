#ifndef SPRITE_H_
#define SPRITE_H_
#include "olc/olc.h"

typedef struct {
    int width;
    int height;
    short* colours;
} sprite_t;

void* CheckNull(void* ptr);

void init_sprite(int width, int height, sprite_t* sprite);
void deinit_sprite(sprite_t* sprite);

short get_sprite_color(int x, int y, sprite_t* sprite);
void set_sprite_color(int x, int y, sprite_t* sprite, short color);

int load_sprite_from_file(const char* filename, sprite_t* sprite);
int save_sprite_to_file(const char* filename, sprite_t* sprite);

#endif
