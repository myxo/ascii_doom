#pragma warning(disable 4996)
#include "sprite.h"
#include "olc/olc.h"
#include <stdio.h>
#include <windows.h>

typedef struct {
    int size;
    int pos_x;
    int pos_y;
    short col;
    short col_bg;
    short col_fg;
    char glyph;
    short editor_col;
} brush_t;

typedef struct {
    int select;
    int menu_num;
} menu_t;

texture_t texture;
menu_t menu;
int width = 50;
int height = 30;
int glyph_size = 20;
brush_t brush;

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
    HOTKEYS = 7,
};

void init_brush(int x1, int x2, int y1, int y2) {
    brush.pos_x = x1 - 1;
    brush.pos_x = y1 - 1;
    brush.size = 1;
    brush.col = FG_WHITE;
    brush.col_fg = FG_WHITE;
    brush.col_bg = BG_BLACK;
    brush.glyph = '*';
    brush.editor_col = BG_BLACK;
}

void preview(int x, int y) {
    for (int i = y; i < y + texture.height; i++) {
        for (int j = x; j < x + texture.width; j++) {
            olc_draw(j, i, get_texture_glyph(j - x, i - y, &texture), get_texture_color(j - x, i - y, &texture));
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
    int x1, x2, y1, y2;
    x1 = brush.pos_x + 1;
    x2 = x1 + brush.size, texture.width;
    y1 = brush.pos_y+ 1;
    y2 = y1 + brush.size, texture.height;
    olc_fill(x1, y1, x2, y2, brush.glyph, brush.col);
}

void draw_menu() {
    if (menu.menu_num == START_MENU) {
        olc_draw_string(width / 2 - 7, 1, "TEXTURE EDITOR", FG_RED);
        if (menu.select < 0) {
            menu.select = 2;
        }
        else if (menu.select > 2) {
            menu.select = 0;
        }
        olc_draw_string(width / 2 - 4, 4, "new file", FG_GREY);
        olc_draw_string(width / 2 - 5, 5, "load file", FG_GREY);
        olc_draw_string(width / 2 - 2, 6, "exit", FG_GREY);
        if (menu.select == 0) {
            olc_draw_string(width / 2 - 5, 4, ">new file", FG_GREEN);
        }
        else if (menu.select == 1) {
            olc_draw_string(width / 2 - 6, 5, ">load file", FG_GREEN);
        }
        else if (menu.select == 2) {
            olc_draw_string(width / 2 - 3, 6, ">exit", FG_GREEN);
        }
    }
    else if (menu.menu_num == NEW_FILE) {
        system("cls");
        printf("Enter the width and height of the sprite: \n");
        scanf("%d%d", &texture.width, &texture.height);
        init_texture(texture.width, texture.height, &texture);
        menu.menu_num = EDITOR;
        init_brush(1, 1 + texture.width, 1, 1 + texture.height);
    }
    else if (menu.menu_num == EDITOR) {
        brush.pos_x = CheckBorder(brush.pos_x, texture.width);
        brush.pos_y = CheckBorder(brush.pos_y, texture.height);
        olc_draw(0, 0, 'X', FG_WHITE);
        olc_draw(0, 1 + texture.height, 'X', FG_WHITE);
        olc_draw(1 + texture.width, 0, 'X', FG_WHITE);
        olc_draw(1 + texture.width, 1 + texture.height, 'X', FG_WHITE);
        preview(1, 1);
        show_brush();
        olc_draw_string(height - 8, 0, "press H to open hotkey list", FG_WHITE);
    }
    else if (menu.menu_num == LOAD_FILE) {
        system("cls");
        printf("Enter name of file: \n");
        char filename[1024];
        scanf("%s", &filename);
        load_texture_from_file(filename, &texture);
        menu.menu_num = EDITOR;
        init_brush(1, 1 + texture.width, 1, 1 + texture.height);
    }
    else if (menu.menu_num == MAIN_MENU) {
        olc_draw_string(width / 2 - 7, 1, "TEXTURE EDITOR", FG_RED);
        if (menu.select < 0) {
            menu.select = 5;
        }
        else if (menu.select > 4) {
            menu.select = 0;
        }
        olc_draw_string(width / 2 - 4, 4, "save file", FG_GREY);
        olc_draw_string(width / 2 - 4, 5, "load file", FG_GREY);
        olc_draw_string(width / 2 - 4, 6, "new file", FG_GREY);
        olc_draw_string(width / 2 - 7, 7, "return to edit", FG_GREY);
        olc_draw_string(width / 2 - 2, 9, "exit", FG_GREY);
        if (menu.select == 0) {
            olc_draw_string(width / 2 - 5, 4, ">save file", FG_GREEN);
        }
        else if (menu.select == 1) {
            olc_draw_string(width / 2 - 5, 5, ">load file", FG_GREEN);
        }
        else if (menu.select == 2) {
            olc_draw_string(width / 2 - 5, 6, ">new file", FG_GREEN);
        }
        else if (menu.select == 3) {
            olc_draw_string(width / 2 - 8, 7, ">return to edit", FG_GREEN);
        }
        else if (menu.select == 4) {
            olc_draw_string(width / 2 - 3, 9, ">exit", FG_GREEN);
        }
    }
    else if (menu.menu_num == SAVE_FILE) {
        system("cls");
        printf("Enter name of file: \n");
        char filename[1024];
        scanf("%s", &filename);
        save_texture_to_file(filename, &texture);
        menu.menu_num = EDITOR;
        init_brush(1, 1 + texture.width, 1, 1 + texture.height);
    }
    else if (menu.menu_num == CHOOSE_GLYPH){
        system("cls");
        printf("Enter character for brush: \n");
        brush.glyph = getc(stdin);
        while (brush.glyph == '\n')
        {
            brush.glyph = getc(stdin);
        }
        menu.menu_num = EDITOR;
    }
    else if (menu.menu_num == HOTKEYS) {
        system("cls");
        printf("HOTKEYS: \n");
        printf("Q/E : change glyph of brush \n");
        printf("W/S : change FG colour of brush \n");
        printf("A/D : change BG colour of brush \n");
        printf("R: get colour and glyph to brush \n");
        printf("-/+ : change size of brush \n");
        printf("LShift: set glyph of brush \n");
        printf("Tab: set null as a glyph of brush\n");
        printf("Spacebar: draw\n");
        printf("F set BG colour of brush as BG colour of editor\n");
        printf("H: show hotkeys list\n");
        printf("Esc: open menu\n");
        system("pause");
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
        }
        else if (menu.select == 2) {
            exit(0);
        }
    }
    if (menu.menu_num == EDITOR) {
        for (int i = 0; i < brush.size; i++) {
            for (int j = 0; j < brush.size; j++) {
                if (brush.pos_x + j < texture.width && brush.pos_y + i < texture.height) {
                    set_texture_color(brush.pos_x + j, brush.pos_y + i, &texture, brush.col);
                    set_texture_glyph(brush.pos_x + j, brush.pos_y + i, &texture, brush.glyph);
                }
            }
        }
        set_texture_color(brush.pos_x, brush.pos_y, &texture, brush.col);
        set_texture_glyph(brush.pos_x, brush.pos_y, &texture, brush.glyph);
    }
    if (menu.menu_num == MAIN_MENU) {
        if (menu.select == 0)   {
            menu.menu_num = SAVE_FILE;
        }
        if (menu.select == 1) {
            menu.menu_num = LOAD_FILE;
        }
        if (menu.select == 2) {
            deinit_texture(&texture);
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
        if (olc_get_key(VK_ESCAPE).held) {
            menu.menu_num = MAIN_MENU;
            menu.select = 0;
        }
        if (olc_get_key(VK_TAB).pressed) {
            brush.glyph = '\0';
        }
        if (olc_get_key('W').pressed) { //W key
            if (brush.col_fg >= 15) {
                brush.col_fg = 0;
            }
            else {
                brush.col_fg++;
            }
            brush.col = (brush.col_bg * 16) + brush.col_fg;
        }
        if (olc_get_key('S').pressed) { //S key
            if (brush.col_fg <= 0) {
                brush.col_fg = 15;
            }
            else {
                brush.col_fg--;
            }
            brush.col = (brush.col_bg * 16) + brush.col_fg;
        }
        if (olc_get_key('D').pressed) { //D key
            if (brush.col_bg >= 15) {
                brush.col_bg = 0;
            }
            else {
                brush.col_bg++;
            }
            brush.col = (brush.col_bg * 16) + brush.col_fg;
        }
        if (olc_get_key('R').pressed) { //R key
            brush.glyph = get_texture_glyph(brush.pos_x, brush.pos_y, &texture);
            brush.col = get_texture_color(brush.pos_x, brush.pos_y, &texture);
            brush.col_bg = brush.col / 16;
            brush.col_fg = brush.col % 16;
        }
        if (olc_get_key('A').pressed) { //A key
            if (brush.col_bg <= 0) {
                brush.col_bg = 15;
            }
            else {
                brush.col_bg--;
            }
            brush.col = (brush.col_bg * 16) + brush.col_fg;
        }
        if (olc_get_key('E').pressed) { //E key
            if (brush.glyph >= 255) {
                brush.glyph = 0;
            }
            else {
                brush.glyph++;
            }
        }
        if (olc_get_key('Q').pressed) { //Q key
            if (brush.glyph <= 0) {
                brush.glyph = 255;
            }
            else {
                brush.glyph--;
            }
        }
        if (olc_get_key('F').pressed) {
            brush.editor_col = brush.col_bg * 16;
        }
        if (olc_get_key('H').pressed) {
            menu.menu_num = HOTKEYS;
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
    }
}

int update(float time_elapsed) {
    handle_input(time_elapsed);
    if (stop) {
        return 0;
    }
    if (menu.menu_num == EDITOR) {
        olc_fill(0, 0, width, height, ' ', brush.editor_col);
    }
    else {
        olc_fill(0, 0, width, height, ' ', FG_BLACK);
    }
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
    return 0;
}

