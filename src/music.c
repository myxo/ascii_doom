#include "olc/olc.h"
#include "world_object.h"
#include "music.h"
#include "logging.h"

void update_music(world_t* world) {
    if (!olc_is_sample_playing(world->music.current_id) && !world->is_mute) {
        int index = world->music.current_index;
        index++;
        if (index >= world->music.len) {
            index -= world->music.len;
        }
        world->music.current_index = index;
        world->music.current_id = world->music.music_array[world->music.current_index];
        olc_play_sound(world->music.current_id);
    }
}
