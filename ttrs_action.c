#include <limits.h>
#include <ncurses.h>
#include <stdbool.h>
#include <string.h>

#include "tetris.h"
#include "ttrs_action.h"
#include "ttrs_field.h"
#include "ttrs_mino.h"
#include "ttrs_print.h"

t_keyhook_func g_keyhooks[UCHAR_MAX] = {};

#define DOWN(pos) pos.row + 1, pos.col
#define LEFT(pos) pos.row, pos.col - 1
#define RIGHT(pos) pos.row, pos.col + 1
#define POS(pos) pos.row, pos.col

void assign_keyhook_funcp()
{
	g_keyhooks[DOWN_KEY] = try_down;
	g_keyhooks[LEFT_KEY] = try_left;
	g_keyhooks[RIGHT_KEY] = try_right;
	g_keyhooks[ROTATE_KEY] = try_spin;
	g_keyhooks[SPACE_KEY] = down_direction;
}

t_status try_down(t_tetris *tetris, t_mino *mino)
{
	(void)tetris;
	if (can_place_in_field(tetris->field, &mino->mino_type, DOWN(mino->pos))) {
		mino->pos.row += 1;
	} else {
		return TETRIS_BOTTOM;
	}
	return TETRIS_FALL;
}

t_status try_left(t_tetris *tetris, t_mino *mino)
{
	(void)tetris;
	if (can_place_in_field(tetris->field, &mino->mino_type, LEFT(mino->pos))) {
		mino->pos.col -= 1;
	}
	return TETRIS_FALL;
}

t_status try_right(t_tetris *tetris, t_mino *mino)
{
	(void)tetris;
	if (can_place_in_field(tetris->field, &mino->mino_type, RIGHT(mino->pos))) {
		mino->pos.col += 1;
	}
	return TETRIS_FALL;
}

t_status try_spin(t_tetris *tetris, t_mino *mino)
{
	t_mino spined = *mino;
	spin_right(&spined.mino_type);
	if (can_place_in_field(tetris->field, &spined.mino_type, POS(spined.pos))) {
		*mino = spined;
	}
	return TETRIS_FALL;
}

t_status down_direction(t_tetris *tetris, t_mino *mino)
{
	while (can_place_in_field(tetris->field, &mino->mino_type, DOWN(mino->pos))) {
		mino->pos.row += 1;
	}
	return TETRIS_BOTTOM;
}

t_status create_new_mino(t_field field, t_mino *mino)
{
	*mino = generate_random_mino();
	if (!can_place_in_field(field, &mino->mino_type, POS(mino->pos))) {
		return TETRIS_GAME_OVER;
	}
	return TETRIS_FALL;
}

t_status handle_key_input(t_tetris *tetris, t_mino *mino)
{
	int c = getch();
	if (c != ERR && g_keyhooks[c]) {
		return g_keyhooks[c](tetris, mino);
	}
	return TETRIS_FALL;
}
