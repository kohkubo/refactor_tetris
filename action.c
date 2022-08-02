#include "tetris.h"

#include <string.h>
#include <ncurses.h>

extern t_keyhook_func g_keyhooks[UCHAR_MAX];

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

bool try_move_down(t_tetris *tetris, t_mino *mino)
{
	if (can_place_in_field(tetris->field_ptr, &mino->mino_type, down(mino->pos)))
	{
		mino->pos = down(mino->pos);
		return true;
	}
	return false;
}

bool try_move_left(t_tetris *tetris, t_mino *mino)
{
	if (can_place_in_field(tetris->field_ptr, &mino->mino_type, left(mino->pos)))
	{
		mino->pos = left(mino->pos);
		return true;
	}
	return false;
}

bool try_move_right(t_tetris *tetris, t_mino *mino)
{
	if (can_place_in_field(tetris->field_ptr, &mino->mino_type, right(mino->pos)))
	{
		mino->pos = right(mino->pos);
		return true;
	}
	return false;
}

bool try_move_rotate(t_tetris *tetris, t_mino *mino)
{
	t_mino temp = *mino;
	rotate_right(&temp.mino_type);
	if (can_place_in_field(tetris->field_ptr, &temp.mino_type, temp.pos))
	{
		rotate_right(&mino->mino_type);
		return true;
	}
	return false;
}

void handle_key_input(t_tetris *tetris, t_mino *mino)
{
	int c = getch();
	if (c != ERR && g_keyhooks[c])
	{
		g_keyhooks[c](tetris, mino);
		update_screen(tetris, mino);
	}
}
