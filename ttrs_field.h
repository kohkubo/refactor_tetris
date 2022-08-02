#ifndef TTRS_FIELD_H
#define TTRS_FIELD_H

#include "tetris.h"

bool can_place_in_field(const t_field field, const t_mino_type *mino_type, const t_point dest);
void place_mino_on_field(t_field field, t_mino *mino);
int erase_filled_lines(t_field field);

#endif
