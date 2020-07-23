#ifndef PLAYER_H_
#define PLAYER_H_

void move_player(int forward, int right, float time_elapsed);
void turn_player(int dir, double time_elapsed);
void reset_player_angular_speed();
void player_hit(double damage);
void player_regen(double time_elapsed);

#endif 
