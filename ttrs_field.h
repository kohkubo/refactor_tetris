#ifndef TTRS_FIELD_HPP
#define TTRS_FIELD_HPP

#include "tetris.h"

bool can_place_in_field(const t_field_ptr field_ptr,
						const t_mino_type *mino_type, const t_point dest);
void place_mino_on_field(t_field_ptr current_field, t_mino *mino);
int erase_filled_lines(t_field_ptr field_ptr);

#endif /* TTRS_FIELD_HPP */
