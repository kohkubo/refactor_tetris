#ifndef TTRS_FIELD_HPP
#define TTRS_FIELD_HPP

#include "tetris.h"

bool can_place_in_field(const t_field field, const t_mino_type *mino_type, int row, int col);
void place_mino_on_field(t_field field, t_mino *mino);
int erase_filled_lines(t_field field);

#endif /* TTRS_FIELD_HPP */
