#include "tetris.h"

extern t_field g_field;
extern t_mino g_current;

static t_point down(t_point pos)
{
	return (t_point){pos.row + 1, pos.col};
}

static t_point left(t_point pos)
{
	return (t_point){pos.row, pos.col - 1};
}

static t_point right(t_point pos)
{
	return (t_point){pos.row, pos.col + 1};
}

bool try_move_down()
{
	if (can_place_in_field(g_current, down(g_current.pos)))
	{
		g_current.pos = down(g_current.pos);
		return true;
	}
	return false;
}

bool try_move_left()
{
	if (can_place_in_field(g_current, left(g_current.pos)))
	{
		g_current.pos = left(g_current.pos);
		return true;
	}
	return false;
}

bool try_move_right()
{
	if (can_place_in_field(g_current, right(g_current.pos)))
	{
		g_current.pos = right(g_current.pos);
		return true;
	}
	return false;
}

bool try_move_rotate()
{
	t_mino temp = copy_mino(g_current);
	rotate_right(temp);
	if (can_place_in_field(temp, temp.pos))
	{
		rotate_right(g_current);
		free_mino(temp);
		return true;
	}
	free_mino(temp);
	return false;
}
