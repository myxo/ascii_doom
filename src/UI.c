#include "../third_party/olc/olc.h"
#include "UI.h"

#include <cassert>
#include <stdlib.h>

canvas_t* init_canvas(update_t update) {
    canvas_t* canvas = malloc(sizeof(canvas_t));
    canvas->buttons = malloc(sizeof(button_t));
    canvas->num_of_buttons = 0;
    canvas->is_active = 0;
    canvas->update = update;
    return canvas;
}

void deinit_canvas(canvas_t* canvas) {
    free(canvas->buttons);
    free(canvas);
}

void add_button(canvas_t* canvas, int x, int y, int width, int height, char* label, action_t action) {
    canvas->buttons = realloc(canvas->buttons, ++canvas->num_of_buttons * sizeof(button_t));
    button_t button;
    button.x = x;
    button.y = y;
    button.width = width;
    button.height = height;
    button.label = label;
    char tmp;
    int i = 0;
    do {
        tmp = label[i++];
    } while (tmp != 0);
    button.label_size = i - 1;
    button.is_active = 0;
    button.action = action;
    canvas->buttons[canvas->num_of_buttons - 1] = button;
}

int get_active_button_id(canvas_t* canvas) {
    int i = 0;
    for (; i < canvas->num_of_buttons && !canvas->buttons[i].is_active; i++);
    return i;
}

button_t* get_active_button(canvas_t* canvas) {
    return &canvas->buttons[get_active_button_id(canvas)];
}

void set_active_button_by_id(canvas_t* canvas, int index) {
    assert(index < canvas->num_of_buttons);
    assert(index >= 0);
    int prev = get_active_button_id(canvas);
    canvas->buttons[prev].is_active = 0;
    canvas->buttons[index].is_active = 1;

}

void draw_canvas(canvas_t* canvas) {
    for (int i = 0; i < canvas->num_of_buttons; i++) {
        short color = BG_BLACK;
        if (canvas->buttons[i].is_active) {
            color = BG_DARK_GREY;
        }
        for (int j = 0; j < canvas->buttons[i].width; j++) {
            for (int k = 0; k < canvas->buttons[i].height; k++) {
                olc_draw(canvas->buttons[i].x + j, canvas->buttons[i].y + k, ' ', color);
            }
        }
        int label_x = canvas->buttons[i].x + (canvas->buttons[i].width - canvas->buttons[i].label_size) / 2;
        int label_y = canvas->buttons[i].y + canvas->buttons[i].height / 2;
        olc_draw_string(label_x, label_y, canvas->buttons[i].label, FG_WHITE + color);
    }
}
