#include "config.h"

#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
typedef struct {
    config_item_t item;
    char* name;
    double value;
} config_description_t;

config_description_t config_description[2] = {
    {kPlayerSpeed, "player_speed"},
    {kViewAngle, "view_angle"}
};

void read_config_from_file(const char filename[50]) {
    FILE* cfg;
    cfg = fopen(filename,"r");
    char name_value[20];
    double value;
    for (int i = 0; i < 2; i++) {
        fscanf(cfg, "%s%lf", name_value, &value);
        for (int j = 0; j < 2; j++) {
            if (strcmp(config_description[j].name, name_value) == 0) {
                config_description[j].value = value;
                break;
            }
        }
        
    }

    fclose(cfg);
}

double get_config_value(config_item_t item) {
    for (int i = 0; i < 2; i++) {
        if (config_description[i].item == item) {
            return config_description[i].value;
        }
    }
    return 0;
}

