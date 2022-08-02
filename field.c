#include "tetris.h"

#include <string.h>

extern t_field g_field;
extern t_mino g_current;
extern long g_turn_time_nanosec;

bool can_place_in_field(t_mino mino, t_point dest)
{
	char **shape = mino.mino_shape.shape;
	for (int i = 0; i < (int)mino.mino_shape.width; i++)
	{
		for (int j = 0; j < (int)mino.mino_shape.width; j++)
		{
			if (!shape[i][j])
			{
				continue;
			}
			if (dest.col + j < 0 ||
				dest.col + j >= COL ||
				dest.row + i >= ROW ||
				g_field[dest.row + i][dest.col + j])
			{
				return false;
			}
		}
	}
	return true;
}

void update_field(t_field field)
{
	t_point pos = g_current.pos;
	t_mino_shape shape = g_current.mino_shape;
	for (size_t i = 0; i < shape.width; i++)
	{
		for (size_t j = 0; j < shape.width; j++)
		{
			if (shape.shape[i][j])
			{
				field[pos.row + i][pos.col + j] = shape.shape[i][j];
			}
		}
	}
}

static void erase_filled_line(int row)
{
	for (size_t i = row; i > 0; i--)
	{
		memcpy(g_field[i], g_field[i - 1], COL);
	}
	memset(g_field[0], 0, COL);
}

static bool is_filled_line(int row)
{
	size_t filled_cell = 0;
	for (size_t i = 0; i < COL; i++)
	{
		filled_cell += g_field[row][i];
	}
	return filled_cell == COL;
}

size_t handle_filled_lines()
{
	size_t count = 0;
	for (size_t i = 0; i < ROW; i++)
	{
		if (is_filled_line(i))
		{
			count++;
			erase_filled_line(i);
		}
	}
	return count;
}
