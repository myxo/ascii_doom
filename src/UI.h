#ifndef UI_H_
#define UI_H_

typedef int (*update_t)(float);

typedef struct {
    int x;
    int y;
    int width;
    int height;
    int is_active;
    char* label;
} button_t;

typedef struct {
    button_t* buttons;
    int num_of_buttons;
    update_t update;
} canvas_t;

void add_button(canvas_t* canvas, int x, int y, int width, int height, char* label);
void deinit_canvas(canvas_t* canvas);
canvas_t* init_canvas(update_t update);
void draw_canvas(canvas_t* canvas);
int get_active_button_id(canvas_t* canvas);
void set_active_button_by_id(canvas_t* canvas, int index);

#endif // !UI_H_
