#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
typedef struct{
 struct vec position;
} player;
player main_player;
void set_position_player(struct vec pos);
void move_player(struct vec position_move);
void save_player();
void load_player();
void fall_player();
#endif // PLAYER_H_INCLUDED
