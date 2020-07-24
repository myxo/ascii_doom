#include "sprite.h"
#include "world_object.h"
#include <math.h>
#include "player.h"

void move_player(int forward, int right, float time_elapsed) {
    world_t* world = get_world();

    int is_wall_reached, is_door_reached;

    double new_x = world->player.pos.x;
    new_x += forward * time_elapsed * world->player.speed * sin(world->player.angle);
    new_x += right * time_elapsed * world->player.speed * cos(world->player.angle);
    is_wall_reached = is_wall_in_radius(new_x, world->player.pos.y, world->player.radius);
    is_door_reached = is_door_in_radius(new_x, world->player.pos.y, world->player.radius);
    if (!is_wall_reached && !is_door_reached && fabs(new_x - world->player.pos.x) > 0.015)
        world->player.pos.x = new_x;

    double new_y = world->player.pos.y;
    new_y += forward * time_elapsed * world->player.speed * cos(world->player.angle);
    new_y -= right * time_elapsed * world->player.speed * sin(world->player.angle);
    is_wall_reached = is_wall_in_radius(world->player.pos.x, new_y, world->player.radius);
    is_door_reached = is_door_in_radius(world->player.pos.x, new_y, world->player.radius);
    if (!is_wall_reached && !is_door_reached && fabs(new_y - world->player.pos.y) > 0.015)
        world->player.pos.y = new_y;
}

void turn_player(int dir, double time_elapsed) {
    world_t* world = get_world();
    world->player.angular_speed *= world->player.angular_speed_multi;
    world->player.angle += dir * time_elapsed * world->player.angular_speed;
    world->player.angular_speed_multi += time_elapsed * world->player.angular_acceleration;
}

void reset_player_angular_speed() {
    world_t* world = get_world();
    world->player.angular_speed_multi = 1;
}

void player_hit(double damage) {
    world_t* world = get_world();;
    world->player.health -= damage;
}

void player_regen(double time_elapsed) {
    world_t* world = get_world();
    world->player.health += world->player.regen * time_elapsed;
}

