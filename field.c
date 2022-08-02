#include "tetris.h"

#include <string.h>

static bool is_in_field(const t_point *dest, int row, int col)
{
	return (dest->col + col >= 0 && dest->col + col < COL && dest->row + row < ROW);
}

bool can_place_in_field(t_field_ptr field, const t_mino_shape *mino_shape, const t_point dest)
{
	char **shape = mino_shape->shape;
	for (int i = 0; i < (int)mino_shape->width; i++)
	{
		for (int j = 0; j < (int)mino_shape->width; j++)
		{
			if (!shape[i][j])
			{
				continue;
			}
			if (!is_in_field(&dest, i, j) || field[dest.row + i][dest.col + j])
			{
				return false;
			}
		}
	}
	return true;
}

void update_field(t_field_ptr current_field, t_mino *mino)
{
	t_point pos = mino->pos;
	t_mino_shape shape = mino->mino_shape;
	for (size_t i = 0; i < shape.width; i++)
	{
		for (size_t j = 0; j < shape.width; j++)
		{
			if (shape.shape[i][j])
			{
				current_field[pos.row + i][pos.col + j] = shape.shape[i][j];
			}
		}
	}
}

static void erase_filled_line(t_field_ptr field, int row)
{
	for (size_t i = row; i > 0; i--)
	{
		memcpy(field[i], field[i - 1], COL);
	}
	memset(field[0], 0, COL);
}

static bool is_filled_line(t_field_line_ptr line)
{
	size_t filled_cell = 0;
	for (size_t i = 0; i < COL; i++)
	{
		filled_cell += line[i];
	}
	return filled_cell == COL;
}

size_t handle_filled_lines(t_field_ptr field)
{
	size_t count = 0;
	for (size_t i = 0; i < ROW; i++)
	{
		if (is_filled_line(field[i]))
		{
			count++;
			erase_filled_line(field, i);
		}
	}
	return count;
}
