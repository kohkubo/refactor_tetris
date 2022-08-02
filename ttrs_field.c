#include "ttrs_field.h"

#include <string.h>

static bool is_in_field(const t_point *dest, int row, int col)
{
	return (dest->col + col >= 0 && dest->col + col < FIELD_COL &&
			dest->row + row < FIELD_ROW);
}

bool can_place_in_field(const t_field_ptr field_ptr,
						const t_mino_type *mino_type, const t_point dest)
{
	for (int i = 0; i < (int)mino_type->width; i++) {
		for (int j = 0; j < (int)mino_type->width; j++) {
			if (!mino_type->shape[i][j]) {
				continue;
			}
			if (!is_in_field(&dest, i, j) || field_ptr[dest.row + i][dest.col + j]) {
				return false;
			}
		}
	}
	return true;
}

void place_mino_on_field(t_field_ptr current_field, t_mino *mino)
{
	t_point pos = mino->pos;
	for (size_t i = 0; i < mino->mino_type.width; i++) {
		for (size_t j = 0; j < mino->mino_type.width; j++) {
			if (mino->mino_type.shape[i][j]) {
				current_field[pos.row + i][pos.col + j] = mino->mino_type.shape[i][j];
			}
		}
	}
}

static void erase_filled_line(t_field_ptr field_ptr, int row)
{
	for (int i = row; i > 0; i--) {
		memcpy(field_ptr[i], field_ptr[i - 1], FIELD_COL);
	}
	memset(field_ptr[0], 0, FIELD_COL);
}

static bool is_filled_line(t_field_line_ptr line)
{
	size_t filled_cell = 0;
	for (size_t i = 0; i < FIELD_COL; i++) {
		filled_cell += line[i];
	}
	return filled_cell == FIELD_COL;
}

size_t erase_filled_lines(t_field_ptr field_ptr)
{
	size_t count = 0;
	for (size_t i = 0; i < FIELD_ROW; i++) {
		if (is_filled_line(field_ptr[i])) {
			count++;
			erase_filled_line(field_ptr, i);
		}
	}
	return count;
}
