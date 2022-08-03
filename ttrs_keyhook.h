#ifndef TTRS_KEYHOOK_HPP
#define TTRS_KEYHOOK_HPP

#include "tetris.h"
#include "ttrs_mino.h"

void handle_key_input(const t_tetris *tetris, t_mino *mino);
bool try_move_down(const t_tetris *tetris, t_mino *mino);
bool try_move_left(const t_tetris *tetris, t_mino *mino);
bool try_move_right(const t_tetris *tetris, t_mino *mino);
bool try_spin(const t_tetris *tetris, t_mino *mino);

#endif /* TTRS_KEYHOOK_HPP */
