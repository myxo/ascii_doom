#ifndef SPRITE_H_
#define SPRITE_H_

typedef struct {
    int width;
    int height;
    short* colours;
    char* glyphs;
} texture_t;

typedef struct {
    int texture_count;
    int capacity;
    texture_t* texture;
} sprite_t;

char get_texture_glyph(int x, int y, texture_t* texture);
void set_texture_glyph(int x, int y, texture_t* texture, char glyph);

void init_texture(int width, int height, texture_t* texture);
void deinit_texture(texture_t* texture);

short get_texture_color(int x, int y, texture_t* texture);
short sample_texture_color(double x, double y, texture_t* texture);
void set_texture_color(int x, int y, texture_t* texture, short color);

void load_texture_from_file(const char* filename, texture_t* texture);
void save_texture_to_file(const char* filename, texture_t* texture);

void init_sprite(sprite_t* sprite);
void deinit_sprite(sprite_t* sprite);
void attach_texture_to_sprite(sprite_t* sprite, texture_t texture);

short get_sprite_color(int x, int y, sprite_t* sprite, int texture_index);
short sample_sprite_color(double x, double y, sprite_t* sprite, int texture_index);
void set_sprite_color(int x, int y, sprite_t* sprite, short color, int texture_index);
char get_sprite_glyph(int x, int y, sprite_t* sprite, int texture_index);
void set_sprite_glyph(int x, int y, sprite_t* sprite, char glyph, int texture_index);

#endif
