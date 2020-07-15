#include "config.h"
#include "olc/olc.h"

#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>

int cfg_draw = 0;
int number_value = 0;
int amount_of_elements = 2;
double delta = 0.5;

typedef struct {
    config_item_t item;
    const char* name;
    double value;
} config_description_t;

config_description_t config_description[2] = {
    {kPlayerSpeed, "player_speed"},
    {kViewAngle, "view_angle"}
};

void read_config_from_file(const char* filename) {
    FILE* cfg = fopen(filename,"r");
    char name_value[256];
    double value;
    int counter = 0;
    for (int i = 0; i < amount_of_elements; i++) {
        fscanf(cfg, "%s%lf", name_value, &value);
        for (int j = 0; j < amount_of_elements; j++) {
            if (strcmp(config_description[j].name, name_value) == 0) {
                config_description[j].value = value;
                counter++;
                break;
            }
        }
        
    }
    for (int i = 0; i < amount_of_elements; i++) {
        if (config_description[i].value == 0) {
            exit(2);
        }
    }
    fclose(cfg);
}

double get_config_value(config_item_t item) {
    for (int i = 0; i < amount_of_elements; i++) {
        if (config_description[i].item == item) {
            return config_description[i].value;
        }
    }
    return 0;
}

void draw_config() {
    if (cfg_draw){
        for (int i = 0; i < amount_of_elements; i++) {
            char val[256];
            gcvt(config_description[i].value, sizeof(double), val);
            if (i == number_value) {
                olc_draw_string(0, i , config_description[i].name,FG_RED | BG_GREEN);
                olc_draw_string(strlen(config_description[i].name), i, val, FG_RED | BG_GREEN);
            }
            else {
                olc_draw_string(0, i, config_description[i].name, FG_RED | BG_GREY);
                olc_draw_string(strlen(config_description[i].name), i, val, FG_RED | BG_GREY);
            }
            
        }
    }
}

void update_config(float time_elapsed) {
    if (olc_get_key(VK_F1).pressed) {
        if (cfg_draw) {
            cfg_draw = 0;
        }
        else
        {
            cfg_draw = 1;
        }
    }
    if (olc_get_key(0xBB).pressed) {
        number_value--;
        if (number_value == -1){
            number_value = 0;
        }
    }
    if (olc_get_key(0xBD).pressed) {
        number_value++;
        if (amount_of_elements == number_value) {
            number_value = amount_of_elements - 1;
        }
        
    }
    if (olc_get_key(0xDB).pressed) {
        config_description[number_value].value -= delta;
    }
    if (olc_get_key(0xDD).pressed) {
        config_description[number_value].value += delta;
    }
}
