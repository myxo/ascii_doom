#include "sprite.h"
#include "world_object.h"
#include <math.h>

void move_player(int forward, int right, double time_elapsed) {
    world_t* world = get_world();

    double new_x = world->player.pos.x;
    new_x += forward * time_elapsed * world->player.speed * sin(world->player.angle);
    new_x += right * time_elapsed * world->player.speed * cos(world->player.angle);
    if (!is_wall(new_x, world->player.pos.y) && fabs(new_x - world->player.pos.x) > 0.015)
        world->player.pos.x = new_x;

    double new_y = world->player.pos.y;
    new_y += forward * time_elapsed * world->player.speed * cos(world->player.angle);
    new_y -= right * time_elapsed * world->player.speed * sin(world->player.angle);
    if (!is_wall(world->player.pos.x, new_y) && fabs(new_y - world->player.pos.y) > 0.015)
        world->player.pos.y = new_y;
}

void turn_player(int dir, double time_elapsed) {
    world_t* world = get_world();
    world->player.angle += dir * time_elapsed * world->player.angular_speed;
}

void player_hit(int damage) {
    world_t* world = get_world();;
    world->player.health -= damage;
    if (world->player.health <= 0)
        exit(0);
}

