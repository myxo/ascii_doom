#include "logging.h"

#include "olc/olc.h"
#include <stdlib.h>
#include <string.h>

FILE* file;

array_var_to_display_t display_logging;

void log_init(const char* filename) {
    file = fopen(filename, "w+");
}
void log_deinit() {
    fclose(file);
}

FILE* log_file()
{
    return file;
}

void create_display_logging() {
    display_logging.len = 0;
    display_logging.capacity = 2;
    display_logging.array = malloc(display_logging.capacity * sizeof(var_to_display_t));
}

void increase_arr_vars_capacity(array_var_to_display_t* display_logging) {
    display_logging->capacity = display_logging->capacity * 2;
    display_logging->array = realloc(display_logging->array, display_logging->capacity * sizeof(var_to_display_t));
}

void add_watch(const char* label, double value) {
    for (int i = 0; i < display_logging.len; i++) {
        if (strcmp(display_logging.array[i].label, label) == 0) {
            display_logging.array[i].value = value;
            return;
        }
    }
    if (display_logging.len >= display_logging.capacity) {
        increase_arr_vars_capacity(&display_logging);
    }
    display_logging.array[display_logging.len].value = value;
    display_logging.array[display_logging.len++].label = label;
}

void stop_watch(const char* label) {
    int index = -1;
    for (int i = 0; i < display_logging.len; i++) {
        if (strcmp(display_logging.array[i].label, label) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return 0;
    }
    for (int i = index; i < display_logging.len - 1; i++) {
        display_logging.array[i] = display_logging.array[i + 1];
    }
    display_logging.len--;
}

void display_watch() {
    char str[50];
    for (int i = 0; i < display_logging.len; i++) {
        sprintf(str, "%s : %lf", display_logging.array[i].label, display_logging.array[i].value);
        olc_draw_string(0, i, str, FG_WHITE);
    }
}
