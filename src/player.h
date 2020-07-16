#ifndef PLAYER_H_
#define PLAYER_H_

void move_player(int forward, int right, float time_elapsed);
void turn_player(int dir);
void player_hit(int damage);

#endif 
