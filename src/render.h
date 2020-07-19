#ifndef RENDER_H_
#define RENDER_H_

void draw_screen(world_t* world);
void draw_minimap(world_t* world);

void draw_sprite(sprite_t* sprite, int x, int y, double distance, int texture_index, int width, int height);
void draw_hp(world_t* world);

#endif // !RENDER
