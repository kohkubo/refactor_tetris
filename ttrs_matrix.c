#include <string.h>

#include "ttrs_matrix.h"
#include "ttrs_print.h"

static bool is_in_matrix(int row, int col, int offset_row, int offset_col)
{
	return (col + offset_col >= 0 && col + offset_col < MATRIX_COL && row + offset_row < MATRIX_ROW);
}

bool can_place_on_matrix(const t_matrix matrix, const t_mino_type *mino_type, int row, int col)
{
	for (int i = 0; i < (int)mino_type->width; i++) {
		for (int j = 0; j < (int)mino_type->width; j++) {
			if (!mino_type->shape[i][j]) {
				continue;
			}
			if (!is_in_matrix(row, col, i, j) || matrix[row + i][col + j]) {
				return false;
			}
		}
	}
	return true;
}

void update_matrix_with_mino(t_matrix matrix, t_mino *mino)
{
	t_point pos = mino->pos;

	for (int i = 0; i < mino->mino_type.width; i++) {
		for (int j = 0; j < mino->mino_type.width; j++) {
			if (mino->mino_type.shape[i][j]) {
				matrix[pos.row + i][pos.col + j] = mino->mino_type.shape[i][j];
			}
		}
	}
}

static void clear_filled_line(t_matrix matrix, int row)
{
	for (int i = row; i > 0; i--) {
		memcpy(matrix[i], matrix[i - 1], MATRIX_COL);
	}
	memset(matrix[0], 0, MATRIX_COL);
}

static bool is_filled_line(t_line line)
{
	int filled_cell = 0;

	for (int i = 0; i < MATRIX_COL; i++) {
		filled_cell += line[i];
	}
	return filled_cell == MATRIX_COL;
}

int clear_filled_lines(t_matrix matrix)
{
	int clear_line_count = 0;

	for (int i = 0; i < MATRIX_ROW; i++) {
		if (is_filled_line(matrix[i])) {
			clear_line_count++;
			clear_filled_line(matrix, i);
		}
	}
	return clear_line_count;
}
