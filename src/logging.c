#include "logging.h"

FILE* file;

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
