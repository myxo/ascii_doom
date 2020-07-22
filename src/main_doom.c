#include "olc/olc.h"
#include "config.h"
#include "sprite.h"
#include "world_object.h"
#include "render.h"
#include "logging.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "drop.h"
#include "map_generator.h"
#include "weapon.h"
#include "rocket.h"
#include "explosion.h"

#include <string.h>
#include <stdio.h>
#include <time.h> 
#include <windows.h>
#include <math.h>
#include <cassert>


int width =  200;
int height = 120;
int glyph_size =  8;
const char config_filename[50] = "cfg.txt";

int create() {
    srand(time(NULL));
    log_init("debug.txt");
    read_config_from_file(config_filename);
    if (init_world_object() == 0) {
        return 0;
    }
    return 1;
}

void handle_player_movement(float time_elapsed) {
    int move_vec_x = 0;
    int move_vec_y = 0;
    if (olc_get_key(VK_LEFT).held) {
        turn_player(-1, time_elapsed);
    }
    if (olc_get_key(VK_RIGHT).held) {
        turn_player(1, time_elapsed);
    }
    if (olc_get_key('W').held) {
        move_vec_x += 1;
    }
    if (olc_get_key('A').held) {
        move_vec_y -= 1;
    }
    if (olc_get_key('S').held) {
        move_vec_x -= 1;
    }
    if (olc_get_key('D').held) {
        move_vec_y += 1;
    }
    if (olc_get_key('1').pressed) {
        set_active_weapon(get_world(), PISTOL);
    }
    if (olc_get_key('2').pressed) {
        set_active_weapon(get_world(), RIFLE);
    }
    if (olc_get_key('3').pressed) {
        set_active_weapon(get_world(), ROCKET_LAUNCHER);
    }
    move_player(move_vec_x, move_vec_y, time_elapsed);
    if (olc_get_key(VK_SPACE).held) {
        shoot_from_active_weapon(get_world());
    }
}


void update_dead_screen() {
    char you_died_str[] = "YOU DIED";
    olc_draw_string(width / 2 - strlen(you_died_str) / 2, height / 2, you_died_str, FG_RED | BG_BLACK);
    if (olc_get_key(VK_SPACE).pressed) {
        deinit_world_object(); // TODO: make reset function
        init_world_object();
    }
}

int update(float time_elapsed) {
    olc_fill(0, 0, width, height, ' ', BG_BLACK);
    if (olc_get_key(VK_ESCAPE).held || olc_get_key(VK_ESCAPE).pressed) {
        return 0;
    }
    world_t* world = get_world();
    if (world->player.health <= 0) {
        update_dead_screen();

    } else {
        if (get_world()->enemy_array.len == 0) {
            add_enemy(get_world());
        }

        handle_config_ui_keypress();
        update_world_from_config();

        handle_player_movement(time_elapsed);
        update_time_since_last_shot(world, time_elapsed);
        bullets_movement(world, time_elapsed);
        rockets_movement(world, time_elapsed);
        enemy_movement(world, time_elapsed);

        update_life_time(world, time_elapsed);

        draw_screen(world);
        draw_minimap(world);
        draw_hp(world);
        display_watch();
        draw_config_ui();
        draw_minimap(world);
        draw_hp(world);
        drop_check(world);
        bullets_counter(world);
        if (world->player.health < world->player.maxhealth) {
          world->player.health += world->player.regen * time_elapsed;
        }
    }
    return 1;
}

int main() {
	if (olc_initialize(width, height, glyph_size, glyph_size) == 0) {
		fprintf(stderr, "Cannot initialize olc");
		return 0;
	}
	olc_register_create(&create);
    olc_register_update(&update);

    olc_start(); // block until update return 0
    olc_deinitialize();
    log_deinit();

    return 0;
}
