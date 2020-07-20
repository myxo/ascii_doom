#ifndef RENDER_H_
#define RENDER_H_

typedef struct {
    int row_left;
    int row_right;
    int line_start;
    int line_end;
    double distance;
} screen_obj_t;

screen_obj_t get_object_on_screen(player_t* player, point_t obj_pos, double obj_radis, double obj_height);

void draw_screen(world_t* world);
void draw_minimap(world_t* world);

void draw_hp(world_t* world);

void draw_sprite(sprite_t* sprite, int texture_index, point_t pos, double obj_radis, double obj_height);

#endif // !RENDER
