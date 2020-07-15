#ifndef CONFIG_H_
#define CONFIG_H_

typedef enum  {
    kPlayerSpeed,
    kViewAngle
}config_item_t;

double get_config_value(config_item_t item);
void read_config_from_file(const char* filename);
void draw_config();
void update_config(float time_elapsed);
#endif
