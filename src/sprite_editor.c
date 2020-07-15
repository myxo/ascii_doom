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
    short col_bg;
    short col_fg;
    char glyph;
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
    CHOOSE_GLYPH = 6,
};

void init_brush(int x1, int x2, int y1, int y2) {
    brush.pos_x = x1 - 1;
    brush.pos_x = y1 - 1;
    brush.size = 1;
    brush.col = FG_WHITE;
    brush.col_fg = FG_WHITE;
    brush.col_bg = BG_BLACK;
    brush.glyph = '*';
};

void preview(int x, int y) {
    for (int i = y; i < y + sprite.height; i++) {
        for (int j = x; j < x + sprite.width; j++) {
            olc_draw(j, i, get_sprite_glyph(j - x, i - y, &sprite), get_sprite_color(j - x, i - y, &sprite));
            olc_draw(j, i, '#', get_sprite_color(j - x, i - y, &sprite));
        }
    }
}
int CheckBorder(int a, int b) {
    if (a >= b) {
        a = 0;
    }
    if (a < 0) {
        a = b - 1;
    }
    return a;
}

void show_brush() {
    //olc_fill(brush.pos_x, brush.pos_y, brush.size + brush.pos_x, brush.size + brush.pos_y, '*', brush.col);
    int x1, x2, y1, y2;
    x1 = brush.pos_x + 1;
    x2 = x1 + brush.size, sprite.width;
    y1 = brush.pos_y+ 1;
    y2 = y1 + brush.size, sprite.height;
    olc_fill(x1, y1, x2, y2, brush.glyph, brush.col);
}

void draw_menu() {
    if (menu.menu_num == START_MENU) {
        olc_draw_string(width / 2 - 6, 1, "SPRITE EDITOR", FG_RED);
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
        menu.menu_num = EDITOR;
        init_brush(1, 1 + sprite.width, 1, 1 + sprite.height);
    }
    else if (menu.menu_num == EDITOR) {
        brush.pos_x = CheckBorder(brush.pos_x, sprite.width);
        brush.pos_y = CheckBorder(brush.pos_y, sprite.height);
        menu.menu_num = 2;
    }
    else if (menu.menu_num == EDITOR) {
        olc_draw(0, 0, 'X', FG_WHITE);
        olc_draw(0, 1 + sprite.height, 'X', FG_WHITE);
        olc_draw(1 + sprite.width, 0, 'X', FG_WHITE);
        olc_draw(1 + sprite.width, 1 + sprite.height, 'X', FG_WHITE);
        preview(1, 1);
        show_brush();
    }
    else if (menu.menu_num == LOAD_FILE) {
        system("cls");
        printf("Enter name of file: \n");
        char filename[1024];
        scanf("%s", &filename);
        load_sprite_from_file(filename, &sprite);
        menu.menu_num = EDITOR;
        init_brush(1, 1 + sprite.width, 1, 1 + sprite.height);
    }
    else if (menu.menu_num == MAIN_MENU) {
        olc_draw_string(width / 2 - 6, 1, "SPRITE EDITOR", FG_RED);
        if (menu.select < 0) {
            menu.select = 5;
        }
        else if (menu.select > 4) {
            menu.select = 0;
        }
        if (menu.select == 0) {
            olc_draw_string(width / 2 - 5, 4, ">save file", FG_GREEN);
            olc_draw_string(width / 2 - 4, 5, "load file", FG_GREY);
            olc_draw_string(width / 2 - 4, 6, "new file", FG_GREY);
            olc_draw_string(width / 2 - 7, 7, "return to edit", FG_GREY);
            olc_draw_string(width / 2 - 2, 9, "exit", FG_GREY);
        }
        else if (menu.select == 1) {
            olc_draw_string(width / 2 - 4, 4, "save file", FG_GREY);
            olc_draw_string(width / 2 - 5, 5, ">load file", FG_GREEN);
            olc_draw_string(width / 2 - 4, 6, "new file", FG_GREY);
            olc_draw_string(width / 2 - 7, 7, "return to edit", FG_GREY);
            olc_draw_string(width / 2 - 2, 9, "exit", FG_GREY);
        }
        else if (menu.select == 2) {
            olc_draw_string(width / 2 - 4, 4, "save file", FG_GREY);
            olc_draw_string(width / 2 - 4, 5, "load file", FG_GREY);
            olc_draw_string(width / 2 - 5, 6, ">new file", FG_GREEN);
            olc_draw_string(width / 2 - 7, 7, "return to edit", FG_GREY);
            olc_draw_string(width / 2 - 2, 9, "exit", FG_GREY);
        }
        else if (menu.select == 3) {
            olc_draw_string(width / 2 - 4, 4, "save file", FG_GREY);
            olc_draw_string(width / 2 - 4, 5, "load file", FG_GREY);
            olc_draw_string(width / 2 - 4, 6, "new file", FG_GREY);
            olc_draw_string(width / 2 - 8, 7, ">return to edit", FG_GREEN);
            olc_draw_string(width / 2 - 2, 9, "exit", FG_GREY);
        }
        else if (menu.select == 4) {
            olc_draw_string(width / 2 - 4, 4, "save file", FG_GREY);
            olc_draw_string(width / 2 - 4, 5, "load file", FG_GREY);
            olc_draw_string(width / 2 - 4, 6, "new file", FG_GREY);
            olc_draw_string(width / 2 - 7, 7, "return to edit", FG_GREY);
            olc_draw_string(width / 2 - 3, 9, ">exit", FG_GREEN);
        }
    }
    else if (menu.menu_num == SAVE_FILE) {
        system("cls");
        printf("Enter name of file: \n");
        char filename[1024];
        scanf("%s", &filename);
        save_sprite_to_file(filename, &sprite);
        menu.menu_num = EDITOR;
        init_brush(1, 1 + sprite.width, 1, 1 + sprite.height);
    }
    else if (menu.menu_num == CHOOSE_GLYPH){
        system("cls");
        printf("Enter character for brush: \n");
        char tmp[4];
        scanf("%s", &tmp);
        if (tmp == '\0') {
            brush.glyph = ' ';
        }
        else {
            brush.glyph = tmp[0];
        }
        menu.menu_num = EDITOR;
    }
}

void press_button() {
    if (menu.menu_num == START_MENU) {
        if (menu.select == 0) {
            menu.menu_num = NEW_FILE;
        }
        else if (menu.select == 1) {
            menu.menu_num = LOAD_FILE;
    if (menu.menu_num == 0) {
        if (menu.select == 0) {
            menu.menu_num = 1;
        }
        else if (menu.select == 2) {
            exit(0);
        }
    }
    if (menu.menu_num == EDITOR) {
        for (int i = 0; i < brush.size; i++) {
            for (int j = 0; j < brush.size; j++) {
                if (brush.pos_x + j < sprite.width && brush.pos_y + i < sprite.height) {
                    set_sprite_color(brush.pos_x + j, brush.pos_y + i, &sprite, brush.col);
                    set_sprite_glyph(brush.pos_x + j, brush.pos_y + i, &sprite, brush.glyph);
                }
            }
        }
        set_sprite_color(brush.pos_x, brush.pos_y, &sprite, brush.col);
        set_sprite_glyph(brush.pos_x, brush.pos_y, &sprite, brush.glyph);
    }
    if (menu.menu_num == MAIN_MENU) {
        if (menu.select == 0)   {
            menu.menu_num = SAVE_FILE;
        }
        if (menu.select == 1) {
            menu.menu_num = LOAD_FILE;
        }
        if (menu.select == 2) {
            deinit_sprite(&sprite);
            menu.menu_num = NEW_FILE;
        }
        if (menu.select == 3) {
            menu.menu_num = EDITOR;
        }
        if (menu.select == 4) {
            exit(0);
        }
    }
}

int create() {
    return 1;
}

void handle_input(float time_elapsed) {
    if (olc_get_key(VK_SPACE).pressed) {
        press_button();
    }
    if (menu.menu_num != EDITOR) {
        if (olc_get_key(VK_UP).pressed) {
            menu.select--;
        }
        if (olc_get_key(VK_DOWN).pressed) {
            menu.select++;
        }
    }
    else
    {
        if (olc_get_key(VK_OEM_MINUS).pressed) {
            if (brush.size > 1) {
                brush.size--;
            }
        }
        if (olc_get_key(VK_OEM_PLUS).pressed) {
            if (brush.size < 10) {
                brush.size++;
            }
        }
        if (olc_get_key(0x1B).held) { // Esc
            menu.menu_num = MAIN_MENU;
            menu.select = 0;
        }
        if (olc_get_key(0x57).pressed) { //W key
            if (brush.col_fg >= 15) {
                brush.col_fg = 0;
            }
            else {
                brush.col_fg++;
            }
            brush.col = (brush.col_bg * 16) + brush.col_fg;
        }
        if (olc_get_key(0x53).pressed) { //S key
            if (brush.col_fg <= 0) {
                brush.col_fg = 15;
            }
            else {
                brush.col_fg--;
            }
            brush.col = (brush.col_bg * 16) + brush.col_fg;
        }
        if (olc_get_key(0x44).pressed) { //D key
            if (brush.col_bg >= 15) {
                brush.col_bg = 0;
            }
            else {
                brush.col_bg++;
            }
            brush.col = (brush.col_bg * 16) + brush.col_fg;
        }
        if (olc_get_key(0x52).pressed) { //R key
            brush.glyph = get_sprite_glyph(brush.pos_x, brush.pos_y, &sprite);
            brush.col = get_sprite_color(brush.pos_x, brush.pos_y, &sprite);
        }
        if (olc_get_key(0x41).pressed) { //A key
            if (brush.col_bg <= 0) {
                brush.col_bg = 15;
            }
            else {
                brush.col_bg--;
            }
            brush.col = (brush.col_bg * 16) + brush.col_fg;
        }
        if (olc_get_key(0x45).pressed) { //E key
            if (brush.glyph >= 255) {
                brush.glyph = 0;
            }
            else {
                brush.glyph++;
            }
        }
        if (olc_get_key(0x51).pressed) { //Q key
            if (brush.glyph <= 0) {
                brush.glyph = 255;
            }
            else {
                brush.glyph--;
            }
        }
        if (olc_get_key(VK_LSHIFT).pressed) {
            menu.menu_num = CHOOSE_GLYPH;
        }
        if (olc_get_key(VK_DOWN).pressed) {
            brush.pos_y++;
        }
        if (olc_get_key(VK_UP).pressed) {
            brush.pos_y--;
        }
        if (olc_get_key(VK_RIGHT).pressed) {
            brush.pos_x++;
        }
        if (olc_get_key(VK_LEFT).pressed) {
            brush.pos_x--;
        }
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

