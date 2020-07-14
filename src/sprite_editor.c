#pragma warning(disable 4996)
#include "sprite.h"
#include "olc/olc.h"
#include <stdio.h>
#include <Windows.h>

typedef struct {
    int canvas[4];
    int size;
    int pos_x;
    int pos_y;
    short col;
} brush_t;

typedef struct {
    int select;
    int menu_num;
} menu_t;

sprite_t sprite;
menu_t menu;
int width = 50;
int height = 30;
int glyph_size = 20;
brush_t brush;
FILE* file;



int stop = 0;

enum menu_nums
{
    START_MENU = 0,
    NEW_FILE = 1,
    EDITOR = 2,
    MAIN_MENU = 3,
    SAVE_FILE = 4,
    LOAD_FILE = 5,
    CHOOSE_COLOUR = 6,
    BRUSH_SIZE = 7,
};

void init_brush(x1, x2, y1, y2) {

}

void preview(int x, int y) {
    for (int i = y; i < y + sprite.height; i++) {
        for (int j = x; j < x + sprite.width; j++) {
            olc_draw(j, i, '#', get_sprite_color(j - x, i - y, &sprite));
        }
    }
}

void draw_menu() {
    if (menu.menu_num == START_MEN) {
        olc_draw_string(width / 2 - 7, 1, "SPRITE EDITOR", FG_RED);
        if (menu.select < 0) {
            menu.select = 2;
        }
        else if (menu.select > 2) {
            menu.select = 0;
        }
        if (menu.select == 0) {
            olc_draw_string(width / 2 - 5, 4, ">new file", FG_GREEN);
            olc_draw_string(width / 2 - 5, 5, "load file", FG_GREY);
            olc_draw_string(width / 2 - 2, 6, "exit", FG_GREY);
        }
        else if (menu.select == 1) {
            olc_draw_string(width / 2 - 4, 4, "new file", FG_GREY);
            olc_draw_string(width / 2 - 6, 5, ">load file", FG_GREEN);
            olc_draw_string(width / 2 - 2, 6, "exit", FG_GREY);
        }
        else if (menu.select == 2) {
            olc_draw_string(width / 2 - 4, 4, "new file", FG_GREY);
            olc_draw_string(width / 2 - 5, 5, "load file", FG_GREY);
            olc_draw_string(width / 2 - 3, 6, ">exit", FG_GREEN);
        }
    }
    else if (menu.menu_num == NEW_FILE) {
        system("cls");
        printf("Enter the width and height of the sprite: \n");
        scanf("%d%d", &sprite.width, &sprite.height);
        init_sprite(sprite.width, sprite.height, &sprite);
        menu.menu_num = 2;
    }
    else if (menu.menu_num == EDITOR) {
        olc_draw(0, 0, 'X', FG_WHITE);
        olc_draw(0, 1 + sprite.height, 'X', FG_WHITE);
        olc_draw(1 + sprite.width, 0, 'X', FG_WHITE);
        olc_draw(1 + sprite.width, 1 + sprite.height, 'X', FG_WHITE);
        preview(1, 1);
    }
}

void press_button() {
    if (menu.menu_num == 0) {
        if (menu.select == 0) {
            menu.menu_num = 1;
        }
        else if (menu.select == 2) {
            exit(0);
        }
    }
}

int create() {
    return 1;
}

void handle_input(float time_elapsed) {
    if (olc_get_key(0x1B).held) { // Esc
        stop = 1;
    }
    if (olc_get_key(VK_SPACE).pressed) {
        press_button();
    }
    if (olc_get_key(VK_UP).pressed) {
        menu.select--;
    }
    if (olc_get_key(VK_DOWN).pressed) {
        menu.select++;
    }
}

int update(float time_elapsed) {
    handle_input(time_elapsed);
    if (stop) {
        return 0;
    }
    olc_fill(0, 0, width, height, ' ', FG_BLACK);
    draw_menu();
    return 1;
}

int main() {
    menu.select = 0;
    menu.menu_num = 0;
    if (olc_initialize(width, height, glyph_size, glyph_size) == 0) {
        fprintf(stderr, "Cannot initialize olc");
        return 0;
    }
    olc_register_create(&create);
    olc_register_update(&update);

    olc_start(); // block until update return 0
    olc_deinitialize();
    system("pause");

    return 0;
}

