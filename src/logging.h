#ifndef LOGGING_H_
#define LOGGING_H_

#include <stdio.h>

void log_init(const char* filename);
void log_deinit();
FILE* log_file();

void add_watch(const char* var_name, double value);
void stop_watch(const char* var_name);
void display_watch();

#endif
