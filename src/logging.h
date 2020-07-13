#ifndef LOGGING_H_
#define LOGGING_H_

#include <stdio.h>

void log_init(const char* filename);
void log_deinit();
FILE* log_file();

#endif
