#include <limits.h>
#include <ncurses.h>
#include <stdbool.h>
#include <string.h>

#include "tetris.h"
#include "ttrs_field.h"
#include "ttrs_keyhook.h"
#include "ttrs_mino.h"
#include "ttrs_print.h"

extern t_keyhook_func g_keyhooks[UCHAR_MAX];

t_mino move_down(const t_mino *mino)
{
	return (t_mino){
		.mino_type = mino->mino_type,
		.pos = {mino->pos.row + 1, mino->pos.col}};
}

t_mino move_left(const t_mino *mino)
{
	return (t_mino){
		.mino_type = mino->mino_type,
		.pos = {mino->pos.row, mino->pos.col - 1}};
}

t_mino move_right(const t_mino *mino)
{
	return (t_mino){
		.mino_type = mino->mino_type,
		.pos = {mino->pos.row, mino->pos.col + 1}};
};

t_mino move_spin(const t_mino *mino)
{
	t_mino spined = *mino;
	spin_right(&spined.mino_type);
	return spined;
}

t_status handle_key_input(t_tetris *tetris, t_mino *mino)
{
	int c = getch();
	if (c != ERR && g_keyhooks[c]) {
		t_mino moved_mino = g_keyhooks[c](mino);
		if (can_place_in_field(tetris->field, &moved_mino.mino_type, moved_mino.pos)) {
			*mino = moved_mino;
		} else if (c == DOWN_KEY) {
			return TETRIS_BOTTOM;
		}
	}
	return TETRIS_FALL;
}
