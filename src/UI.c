#include "../third_party/olc/olc.h"
#include "UI.h"

#include <cassert>
#include <stdlib.h>

canvas_t* init_canvas() {
    canvas_t* canvas = malloc(sizeof(canvas_t));
    canvas->buttons = malloc(sizeof(button_t));
    canvas->num_of_buttons = 0;
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
    button.is_active = 0;
    button.action = action;
    canvas->buttons[canvas->num_of_buttons - 1] = button;
}

int get_active_button_id(canvas_t* canvas) {
    int i = 0;
    for (; i < canvas->num_of_buttons && !canvas->buttons[i].is_active; i++);
    return i;
}

void set_active_button_by_id(canvas_t* canvas, int index) {
    assert(index < canvas->num_of_buttons);
    assert(index >= 0);
    int prev = get_active_button_id(canvas);
    canvas->buttons[prev].is_active = 0;
    canvas->buttons[index].is_active = 1;

}

//void set_active_button_by_id(canvas_t* canvas, int index) {
//    assert(index < canvas->num_of_buttons);
//    assert(index >= 0);
//    int prev = get_active_button_id(canvas);
//    canvas->buttons[prev].is_active = 0;
//    canvas->buttons[index].is_active = 1;
//}

void draw_canvas(canvas_t* canvas) {
    for (int i = 0; i < canvas->num_of_buttons; i++) {
        for (int j = 0; j < canvas->buttons[i].width; j++) {
            for (int k = 0; k < canvas->buttons[i].height; k++) {
                if (canvas->buttons[i].is_active) {
                    olc_draw(canvas->buttons[i].x + j, canvas->buttons[i].y + k, ' ', BG_GREY);
                }
                else {
                    olc_draw(j, k, ' ', BG_BLACK);
                }
            }
        }
        olc_draw_string(canvas->buttons[i].x + canvas->buttons[i].width / 2, canvas->buttons[i].y + canvas->buttons[i].height / 2, canvas->buttons[i].label, FG_WHITE);
    }
}
