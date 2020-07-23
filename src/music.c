#include "olc/olc.h"
#include "world_object.h"
#include "music.h"

void update_music(world_t* world, float time_elapsed) {
    world->music.current_music_time += time_elapsed;
    if (world->music.current_music_time >= world->music.duration_array[world->music.current_index] + 1) {
        world->music.current_music_time = 0;
        int index = world->music.current_index;
        index++;
        if (index >= world->music.len) {
            index -= world->music.len;
        }
        world->music.current_index = index;
        olc_play_sound(world->music.music_array[world->music.current_index]);
    }
}
