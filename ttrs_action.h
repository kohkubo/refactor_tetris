#ifndef TTRS_KEYHOOK_H
#define TTRS_KEYHOOK_H

#include "tetris.h"
#include "ttrs_mino.h"

#define DOWN_KEY 's'
#define LEFT_KEY 'a'
#define RIGHT_KEY 'd'
#define ROTATE_KEY 'w'
#define SPACE_KEY ' '

t_status handle_key_input(t_tetris *tetris, t_mino *mino);
t_status try_soft_drop(t_tetris *tetris, t_mino *mino);
t_status try_left(t_tetris *tetris, t_mino *mino);
t_status try_right(t_tetris *tetris, t_mino *mino);
t_status try_spin(t_tetris *tetris, t_mino *mino);
t_status hard_drop(t_tetris *tetris, t_mino *mino);
t_status create_new_mino(t_matrix matrix, t_mino *mino);
void assign_keyhook_funcp();

typedef t_status (*t_keyhook_func)(t_tetris *tetris, t_mino *);

#endif
