#ifndef SPRITE_H_
#define SPRITE_H_

typedef struct {
    int width;
    int height;
    short* colours;
    char* glyphs;
} sprite_t;

char get_sprite_glyph(int x, int y, sprite_t* sprite);
void set_sprite_glyph(int x, int y, sprite_t* sprite, char glyph);

void init_sprite(int width, int height, sprite_t* sprite);
void deinit_sprite(sprite_t* sprite);

short get_sprite_color(int x, int y, sprite_t* sprite);
short sample_sprite_color(double x, double y, sprite_t* texture);
void set_sprite_color(int x, int y, sprite_t* sprite, short color);

void load_sprite_from_file(const char* filename, sprite_t* sprite);
void save_sprite_to_file(const char* filename, sprite_t* sprite);

#endif
