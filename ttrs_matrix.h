#ifndef TTRS_FIELD_H
#define TTRS_FIELD_H

#include "tetris.h"

bool can_place_in_field(const t_matrix matrix, const t_mino_type *mino_type, int row, int col);
void place_mino_on_field(t_matrix matrix, t_mino *mino);
int clear_filled_lines(t_matrix matrix);

#endif
