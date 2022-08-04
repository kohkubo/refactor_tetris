#ifndef TTRS_ACTION_H
#define TTRS_ACTION_H

#include "tetris.h"
#include "ttrs_mino.h"

#define DOWN_KEY 's'
#define LEFT_KEY 'a'
#define RIGHT_KEY 'd'
#define ROTATE_KEY 'w'
#define SPACE_KEY ' '

t_mino move_down(const t_mino *mino);
t_mino move_left(const t_mino *mino);
t_mino move_right(const t_mino *mino);
t_mino move_spin(const t_mino *mino);
typedef t_mino (*t_mino_move_func)();

#endif
