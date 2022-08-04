#include <string.h>

#include "ttrs_field.h"
#include "ttrs_print.h"

static bool is_in_field(int moved_row, int moved_col, int row, int col)
{
	return (moved_col + col >= 0 && moved_col + col < FIELD_COL && moved_row + row < FIELD_ROW);
}

bool can_place_in_field(const t_field field, const t_mino_type *mino_type, int row, int col)
{
	for (int i = 0; i < (int)mino_type->width; i++) {
		for (int j = 0; j < (int)mino_type->width; j++) {
			if (!mino_type->shape[i][j]) {
				continue;
			}
			if (!is_in_field(row, col, i, j) || field[row + i][col + j]) {
				return false;
			}
		}
	}
	return true;
}

void place_mino_on_field(t_field field, t_mino *mino)
{
	t_point pos = mino->pos;

	for (int i = 0; i < mino->mino_type.width; i++) {
		for (int j = 0; j < mino->mino_type.width; j++) {
			if (mino->mino_type.shape[i][j]) {
				field[pos.row + i][pos.col + j] = mino->mino_type.shape[i][j];
			}
		}
	}
}

static void erase_filled_line(t_field field, int row)
{
	for (int i = row; i > 0; i--) {
		memcpy(field[i], field[i - 1], FIELD_COL);
	}
	memset(field[0], 0, FIELD_COL);
}

static bool is_filled_line(t_field_line line)
{
	int filled_cell = 0;

	for (int i = 0; i < FIELD_COL; i++) {
		filled_cell += line[i];
	}
	return filled_cell == FIELD_COL;
}

int erase_filled_lines(t_field field)
{
	int erased_count = 0;

	for (int i = 0; i < FIELD_ROW; i++) {
		if (is_filled_line(field[i])) {
			erased_count++;
			erase_filled_line(field, i);
		}
	}
	return erased_count;
}
