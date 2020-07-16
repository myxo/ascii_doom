#include "olc/olc.h"


#include "sprite.h"
#include "UI.h"
#include "world_object.h"
#include "render.h"
#include "logging.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"

#include<time.h> 
#include <windows.h>
#include <stdio.h>
#include <math.h>


int width =  200;
int height = 150;
int glyph_size =  8;

int stop = 0;


int create() {
    if (init_world_object() == 0) {
        return 0;
    }
    log_init("debug.txt");
    return 1;
}

//void main_menu_handler(float time_elapsed) {
//    if (olc_get_key(VK_LEFT).held) {
//        
//    }
//    if (olc_get_key(VK_RIGHT).held) {
//        turn_player(1, time_elapsed);
//    }
//}

int update(float time_elapsed) {
    if (get_world()->game_layouts.game->is_active) {
        return get_world()->game_layouts.game->update(time_elapsed);
    }else if (get_world()->game_layouts.main_menu->is_active) {
        return get_world()->game_layouts.main_menu->update(time_elapsed);
    }
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
