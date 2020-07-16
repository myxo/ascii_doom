#include "config.h"
#include "olc/olc.h"

#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <assert.h>

int cfg_draw = 0;
int number_value = 0;
double delta = 0.5;

typedef struct {
    config_item_t item;
    const char* name;
    double value;
} config_description_t;

config_description_t config_description[3] = {
    {kPlayerSpeed, "player_speed"},
    {kBulletSpeed, "bullet_speed"},
    {kBulletRadius, "bullet_radius"}
};

void read_config_from_file(const char* filename) {
    int check[256];
    memset(check, 0, 256);
    
    FILE* cfg = fopen(filename,"r");
    char name_value[256];
    double value;
    int counter = 0;
    while (1) {
        
        if (fscanf(cfg, "%s%lf", name_value, &value) == EOF) {
            break;
        }
        for (int j = 0; j < kConfigItemCount; j++) {
            if (strcmp(config_description[j].name, name_value) == 0) {
                config_description[j].value = value;
                check[j] = 1;
                break;
            }
        }
    }
    for (int i = 0; i < kConfigItemCount; i++) {
        if (check[i] == 0) {
            assert(0);
        }
    }
    fclose(cfg);
}

double get_config_value(config_item_t item) {
    for (int i = 0; i < kConfigItemCount; i++) {
        if (config_description[i].item == item) {
            return config_description[i].value;
        }
    }
    return 0;
}

void draw_config_ui() {
    if (cfg_draw){
        for (int i = 0; i < kConfigItemCount; i++) {
            char val[256];
            sprintf(val, "%s %lf", config_description[i].name, config_description[i].value);
            if (i == number_value) {
                olc_draw_string(0, i , val,FG_RED | BG_GREEN);
            }
            else {
                olc_draw_string(0, i, val, FG_RED | BG_GREY);
            }
            
        }
    }
}

void handle_config_ui_keypress() {
    if (olc_get_key(VK_F1).pressed) {
        cfg_draw = !cfg_draw;
    }
    if (olc_get_key(VK_OEM_PLUS).pressed) {
        number_value--;
        if (number_value == -1){
            number_value = 0;
        }
    }
    if (olc_get_key(VK_OEM_MINUS).pressed) {
        number_value++;
        if (kConfigItemCount == number_value) {
            number_value = kConfigItemCount - 1;
        }
        
    }
    if (olc_get_key(VK_OEM_4).pressed) {
        config_description[number_value].value -= delta;
    }
    if (olc_get_key(VK_OEM_6).pressed) {
        config_description[number_value].value += delta;
    }
}
