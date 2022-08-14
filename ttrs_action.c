#include <limits.h>
#include <ncurses.h>
#include <stdbool.h>
#include <string.h>

#include "tetris.h"
#include "ttrs_action.h"
#include "ttrs_display.h"
#include "ttrs_matrix.h"
#include "ttrs_mino.h"

t_keyhook_func g_keyhooks[UCHAR_MAX + 1] = {};

// clang-format off
#define MINO_DOWN(pos) (t_position){pos.row + 1, pos.col}
#define MINO_LEFT(pos) (t_position){pos.row, pos.col - 1}
#define MINO_RIGHT(pos) (t_position){pos.row, pos.col + 1}
// clang-format on

void init_keyhook_func_ptr_array()
{
	g_keyhooks[DROP_KEY] = try_drop;
	g_keyhooks[LEFT_KEY] = try_left;
	g_keyhooks[RIGHT_KEY] = try_right;
	g_keyhooks[SPIN_KEY] = try_spin;
	// g_keyhooks[HARD_DROP_KEY] = hard_drop;
}

t_status try_drop(t_tetris *tetris, t_current_mino *mino)
{
	t_position moved_pos = MINO_DOWN(mino->pos);
	if (can_place_on_matrix(tetris->matrix, &mino->mino_type, moved_pos)) {
		mino->pos = moved_pos;
		return TETRIS_PLAY;
	}
	return TETRIS_LOCK_DOWN;
}

t_status try_left(t_tetris *tetris, t_current_mino *mino)
{
	t_position moved_pos = MINO_LEFT(mino->pos);
	if (can_place_on_matrix(tetris->matrix, &mino->mino_type, moved_pos))
		mino->pos = moved_pos;
	return TETRIS_PLAY;
}

t_status try_right(t_tetris *tetris, t_current_mino *mino)
{
	t_position moved_pos = MINO_RIGHT(mino->pos);
	if (can_place_on_matrix(tetris->matrix, &mino->mino_type, moved_pos))
		mino->pos = moved_pos;
	return TETRIS_PLAY;
}

t_status try_spin(t_tetris *tetris, t_current_mino *mino)
{
	t_current_mino spined = *mino;

	spin_right(&spined.mino_type);
	if (can_place_on_matrix(tetris->matrix, &spined.mino_type, spined.pos)) {
		*mino = spined;
	}
	return TETRIS_PLAY;
}

t_status hard_drop(t_tetris *tetris, t_current_mino *mino)
{

	while (can_place_on_matrix(tetris->matrix, &mino->mino_type, MINO_DOWN(mino->pos))) {
		mino->pos.row += 1;
	}
	return TETRIS_LOCK_DOWN;
}

t_status try_create_mino(t_matrix matrix, t_current_mino *mino)
{
	*mino = generate_random_mino();
	if (!can_place_on_matrix(matrix, &mino->mino_type, mino->pos)) {
		return TETRIS_GAME_OVER;
	}
	return TETRIS_PLAY;
}

static bool is_valid_key(int key)
{
	if (key < 0 || key > UCHAR_MAX)
		return false;
	return g_keyhooks[key] != NULL;
}

t_status handle_key_input(t_tetris *tetris, t_current_mino *mino)
{
	int key = getch();
	if (key != ERR && is_valid_key(key)) {
		return g_keyhooks[key](tetris, mino);
	}
	return TETRIS_PLAY;
}

t_status handle_auto_drop(t_tetris *tetris, t_current_mino *mino)
{
	if (is_time_to_drop(tetris->time.next_drop_time)) {
		tetris->time.next_drop_time = get_next_drop_time(tetris->time.interval);
		return try_drop(tetris, mino);
	}
	return TETRIS_PLAY;
}
