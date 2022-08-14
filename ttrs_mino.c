#include <stdlib.h>
#include <string.h>

#include "tetris.h"
#include "ttrs_mino.h"

// clang-format off
static const t_mino g_minos[] = {
	{
		.shape = {
			{0, 1, 1},
			{1, 1, 0},
			{0, 0, 0}
		},
		.len = 3,
	},
	{
		.shape = {
			{1, 1, 0},
			{0, 1, 1},
			{0, 0, 0}
		},
		.len = 3
	},
	{
		.shape = {
			{0, 1, 0},
			{1, 1, 1},
			{0, 0, 0}
		},
		.len = 3
	},
	{
		.shape = {
			{0, 0, 1},
			{1, 1, 1},
			{0, 0, 0}
		},
		.len = 3
	},
	{
		.shape = {
			{1, 0, 0},
			{1, 1, 1},
			{0, 0, 0}
		},
	 	.len = 3
	},
	{
		.shape = {
			{1, 1},
			{1, 1}
		},
		.len = 2
	},
	{
		.shape = {
			{0, 0, 0, 0},
			{1, 1, 1, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		.len = 4
	}
};
// clang-format on

void spin_right(t_mino *mino)
{
	t_shape saved_shape;

	memcpy(saved_shape, mino->shape, sizeof(t_shape));
	for (int row = 0; row < mino->len; row++) {
		for (int col = 0, back_col = mino->len - 1; col < mino->len; col++, back_col--) {
			mino->shape[row][col] = saved_shape[back_col][row];
		}
	}
}

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

static t_mino get_random_mino(void)
{
	const int randon_index = rand() % (int)ARRAY_SIZE(g_minos);
	return g_minos[randon_index];
}

static int get_random_col(int mino_length)
{
	return rand() % (MATRIX_COL - mino_length + 1);
}

t_current_mino generate_random_mino(void)
{
	t_current_mino current_mino;

	current_mino.mino = get_random_mino();
	int random_col = get_random_col(current_mino.mino.len);
	current_mino.pos = (t_position){.col = random_col};
	return current_mino;
}
