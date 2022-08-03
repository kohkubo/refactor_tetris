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

t_mino handle_key_input(const t_mino *mino)
{
	int c = getch();
	if (c != ERR && g_keyhooks[c]) {
		return g_keyhooks[c](mino);
	}
	return *mino;
}
