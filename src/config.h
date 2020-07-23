#ifndef CONFIG_H_
#define CONFIG_H_

typedef enum  {
    kPlayerSpeed,
    kPlayerAngularSpeed,
    kBulletSpeed,
    kBulletRadius,
    kAngleOfView,
    kRayTraceStep,
    kFirstAidHeal,
    kPistolAmmo,
    kRifleAmmo,
    kRocketAmmo,
    kConfigItemCount
}config_item_t;

double get_config_value(config_item_t item);
void read_config_from_file(const char* filename);
void draw_config_ui();
void handle_config_ui_keypress();

#endif
