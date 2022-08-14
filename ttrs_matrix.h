#ifndef TTRS_MATRIX_H
#define TTRS_MATRIX_H

#include "tetris.h"

bool can_place_on_matrix(const t_matrix matrix, const t_mino_type *mino_type, t_position pos);
void place_matrix_with_mino(t_matrix matrix, t_current_mino *mino);
int locked_down_current_mino(t_tetris *tetris, t_current_mino *mino);

#endif
