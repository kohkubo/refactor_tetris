#include <limits.h>
#include <ncurses.h>
#include <stdbool.h>
#include <string.h>

#include "tetris.h"
#include "ttrs_field.h"
#include "ttrs_mino.h"
#include "ttrs_print.h"

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

bool try_move_down(const t_tetris *tetris, t_mino *mino)
{
	if (can_place_in_field(tetris->field, &mino->mino_type, down(mino->pos))) {
		mino->pos = down(mino->pos);
		return true;
	}
	return false;
}

bool try_move_left(const t_tetris *tetris, t_mino *mino)
{
	if (can_place_in_field(tetris->field, &mino->mino_type, left(mino->pos))) {
		mino->pos = left(mino->pos);
		return true;
	}
	return false;
}

bool try_move_right(const t_tetris *tetris, t_mino *mino)
{
	if (can_place_in_field(tetris->field, &mino->mino_type, right(mino->pos))) {
		mino->pos = right(mino->pos);
		return true;
	}
	return false;
}

bool try_spin(const t_tetris *tetris, t_mino *mino)
{
	t_mino rotated = *mino;
	spin_right(&rotated.mino_type);
	if (can_place_in_field(tetris->field, &rotated.mino_type, rotated.pos)) {
		mino->mino_type = rotated.mino_type;
		return true;
	}
	return false;
}

// void handle_key_input(const t_tetris *tetris, t_mino *mino)
// {
// 	int c = getch();
// 	if (c != ERR && g_keyhooks[c]) {
// 		g_keyhooks[c](tetris, mino);
// 		update_screen(tetris, mino);
// 	}
// }
