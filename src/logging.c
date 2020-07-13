#include "logging.h"

#include "olc/olc.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    double value;
    char label[30];
} var_to_display_t;

typedef struct {
    var_to_display_t* array;
    int len;
    int capacity;
} array_var_to_display_t;

FILE* file;

array_var_to_display_t display_logging;

void create_display_logging() {
    display_logging.len = 0;
    display_logging.capacity = 2;
    display_logging.array = malloc(display_logging.capacity * sizeof(var_to_display_t));
}


void display_logging_destructor() {
    free(display_logging.array);
}

void log_init(const char* filename) {
    file = fopen(filename, "w+");
    create_display_logging();
}
void log_deinit() {
    fclose(file);
    display_logging_destructor();
}

FILE* log_file()
{
    return file;
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
    strcpy(display_logging.array[display_logging.len].label, label);
    display_logging.len++;
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
        return;
    }
    for (int i = index; i < display_logging.len - 1; i++) {
        display_logging.array[i] = display_logging.array[i + 1];
    }
    display_logging.len--;
}

void display_watch() {
    char** str = malloc(display_logging.len * sizeof(char*));
    int max = 0;
    for (int i = 0; i < display_logging.len; i++) {
        str[i] = malloc(50 * sizeof(char));
        sprintf(str[i], "%s : %lf", display_logging.array[i].label, display_logging.array[i].value);
        int temp = strlen(str[i]);
        if (temp > max) {
            max = temp;
        }
    }
    olc_fill(0, 0, max, display_logging.len, ' ', BG_BLACK);
    for (int i = 0; i < display_logging.len; i++) {
        olc_draw_string(0, i, str[i], FG_WHITE);
        free(str[i]);
    }
    free(str);
}
