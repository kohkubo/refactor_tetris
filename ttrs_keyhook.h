#ifndef TTRS_KEYHOOK_H
#define TTRS_KEYHOOK_H

#include "tetris.h"
#include "ttrs_mino.h"

t_mino handle_key_input(const t_mino *mino);
t_mino move_down(const t_mino *mino);
t_mino move_left(const t_mino *mino);
t_mino move_right(const t_mino *mino);
t_mino move_spin(const t_mino *mino);

typedef t_mino (*t_keyhook_func)(const t_mino *);

#endif
