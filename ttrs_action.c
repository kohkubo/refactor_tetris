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

void init_keyhook_func_ptr_array(void)
{
	g_keyhooks[DROP_KEY] = try_drop;
	g_keyhooks[LEFT_KEY] = try_left;
	g_keyhooks[RIGHT_KEY] = try_right;
	g_keyhooks[SPIN_KEY] = try_spin;
	// g_keyhooks[HARD_DROP_KEY] = hard_drop;
}

t_status try_drop(t_tetris *tetris, t_current_mino *current)
{
	t_position moved_pos = MINO_DOWN(current->pos);
	if (can_place_on_matrix(tetris->matrix, &current->mino, moved_pos)) {
		current->pos = moved_pos;
		return TETRIS_PLAY;
	}
	return TETRIS_LOCK_DOWN;
}

t_status try_left(t_tetris *tetris, t_current_mino *current)
{
	t_position moved_pos = MINO_LEFT(current->pos);
	if (can_place_on_matrix(tetris->matrix, &current->mino, moved_pos))
		current->pos = moved_pos;
	return TETRIS_PLAY;
}

t_status try_right(t_tetris *tetris, t_current_mino *current)
{
	t_position moved_pos = MINO_RIGHT(current->pos);
	if (can_place_on_matrix(tetris->matrix, &current->mino, moved_pos))
		current->pos = moved_pos;
	return TETRIS_PLAY;
}

t_status try_spin(t_tetris *tetris, t_current_mino *current)
{
	t_current_mino spun_mino = *current;

	spin_right(&spun_mino.mino);
	if (can_place_on_matrix(tetris->matrix, &spun_mino.mino, spun_mino.pos)) {
		*current = spun_mino;
	}
	return TETRIS_PLAY;
}

t_status hard_drop(t_tetris *tetris, t_current_mino *current)
{
	while (can_place_on_matrix(tetris->matrix, &current->mino, MINO_DOWN(current->pos))) {
		current->pos.row += 1;
	}
	return TETRIS_LOCK_DOWN;
}

static bool is_valid_key(int key)
{
	if (key < 0 || key > UCHAR_MAX)
		return false;
	return g_keyhooks[key] != NULL;
}

t_status handle_key_input(t_tetris *tetris, t_current_mino *current)
{
	int key = getch();
	if (key != ERR && is_valid_key(key)) {
		return g_keyhooks[key](tetris, current);
	}
	return TETRIS_PLAY;
}

t_status handle_auto_drop(t_tetris *tetris, t_current_mino *current)
{
	if (is_time_to_drop(tetris->time.next_drop_time)) {
		tetris->time.next_drop_time = generate_next_drop_time(tetris->time.interval);
		return try_drop(tetris, current);
	}
	return TETRIS_PLAY;
}
