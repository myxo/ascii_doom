#include "olc/olc.h"
#include "config.h"
#include "sprite.h"
#include "world_object.h"
#include "render.h"
#include "logging.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "weapon.h"

#include <string.h>
#include <stdio.h>
#include <time.h> 
#include <windows.h>
#include <math.h>
#include <cassert>


int width =  200;
int height = 150;
int glyph_size =  8;
const char config_filename[50] = "cfg.txt";
int stop = 0;

int create() {
    read_config_from_file(config_filename);
    if (init_world_object() == 0) {
        return 0;
    }
    log_init("debug.txt");
    return 1;
}

void handle_player_movement(float time_elapsed) {
    point_t move_vec = {0, 0};
    if (olc_get_key(VK_LEFT).held) {
        turn_player(-1, time_elapsed);
    }
    if (olc_get_key(VK_RIGHT).held) {
        turn_player(1, time_elapsed);
    }
    if (olc_get_key('W').held) {
        move_vec.x += 1;
    }
    if (olc_get_key('A').held) {
        move_vec.y -= 1;
    }
    if (olc_get_key('S').held) {
        move_vec.x -= 1;
    }
    if (olc_get_key('D').held) {
        move_vec.y += 1;
    }
    if (olc_get_key('1').pressed) {
        set_active_weapon(get_world(), "pistol");
    }
    if (olc_get_key('2').pressed) {
        set_active_weapon(get_world(), "rifle");
    }
    move_player(move_vec.x, move_vec.y, time_elapsed);
    if (olc_get_key(VK_SPACE).held) {
        shoot_from_active_weapon(get_world());
    }
}


void handle_input(float time_elapsed) {
    if (olc_get_key(VK_ESCAPE).held) {
        stop = 1;
    }
    handle_player_movement(time_elapsed);
}

int update(float time_elapsed) {
    handle_config_ui_keypress();
    update_world_from_config();

	handle_input(time_elapsed);
	if (stop) {
		return 0;
	}
	olc_fill(0, 0, width, height, ' ', BG_BLACK);

    if (get_world()->enemy_array.len == 0) {
        add_enemy(get_world());
    }
    update_time_since_last_shot(get_world(), time_elapsed);
    bullets_movement(get_world(), time_elapsed);
    enemy_movement(get_world(), time_elapsed);
	draw_screen(get_world());
    draw_minimap(get_world());
    draw_sprite(get_world()->textures.wall, 50, 90, 1);
    display_watch();
    draw_config_ui();
	return 1;
}

int main() {
    srand(time(0));
	if (olc_initialize(width, height, glyph_size, glyph_size) == 0) {
		fprintf(stderr, "Cannot initialize olc");
		return 0;
	}
	olc_register_create(&create);
    olc_register_update(&update);

    olc_start(); // block until update return 0
    olc_deinitialize();
    log_deinit();
    deinit_world_object();

    return 0;
}
