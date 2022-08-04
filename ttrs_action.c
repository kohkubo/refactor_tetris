#include <limits.h>
#include <ncurses.h>
#include <stdbool.h>
#include <string.h>

#include "tetris.h"
#include "ttrs_action.h"
#include "ttrs_field.h"
#include "ttrs_mino.h"
#include "ttrs_print.h"


#define DOWN(pos) pos.row + 1, pos.col
#define LEFT(pos) pos.row, pos.col - 1
#define RIGHT(pos) pos.row, pos.col + 1
#define POS(pos) pos.row, pos.col

t_mino move_down(const t_mino *mino)
{
	return (t_mino){
		.mino_type = mino->mino_type,
		.pos = {DOWN(mino->pos)}
	};
}

t_mino move_left(const t_mino *mino)
{
	return (t_mino){
		.mino_type = mino->mino_type,
		.pos = {LEFT(mino->pos)},
	};
}

t_mino move_right(const t_mino *mino)
{
	return (t_mino){
		.mino_type = mino->mino_type,
		.pos = {RIGHT(mino->pos)},
	};
}

t_mino move_spin(const t_mino *mino)
{
	t_mino spined = *mino;
	spin_right(&spined.mino_type);
	return spined;
}
