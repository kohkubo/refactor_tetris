#include <stdlib.h>
#include <string.h>

#include "tetris.h"
#include "ttrs_mino.h"

// clang-format off
const t_mino_type g_mino_types[] = {
	{
		.shape = {
			{0, 1, 1},
			{1, 1, 0},
			{0, 0, 0}
		},
		.width = 3,
	},
	{
		.shape = {
			{1, 1, 0},
			{0, 1, 1},
			{0, 0, 0}
		},
		.width = 3
	},
	{
		.shape = {
			{0, 1, 0},
			{1, 1, 1},
			{0, 0, 0}
		},
		.width = 3
	},
	{
		.shape = {
			{0, 0, 1},
			{1, 1, 1},
			{0, 0, 0}
		},
		.width = 3
	},
	{
		.shape = {
			{1, 0, 0},
			{1, 1, 1},
			{0, 0, 0}
		},
	 	.width = 3
	},
	{
		.shape = {
			{1, 1},
			{1, 1}
		},
		.width = 2
	},
	{
		.shape = {
			{0, 0, 0, 0},
			{1, 1, 1, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		.width = 4
	}
};
// clang-format on

void spin_right(t_mino_type *mino_type)
{
	t_shape saved_shape;

	memcpy(saved_shape, mino_type->shape, sizeof(t_shape));
	for (int row = 0; row < mino_type->width; row++) {
		for (int col = 0, back_col = mino_type->width - 1; col < mino_type->width; col++, back_col--) {
			mino_type->shape[row][col] = saved_shape[back_col][row];
		}
	}
}

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

t_current_mino generate_random_mino()
{
	t_current_mino mino;

	int randon_index = rand() % ARRAY_SIZE(g_mino_types);
	mino.mino_type = g_mino_types[randon_index];
	int random_position = rand() % (MATRIX_COL - mino.mino_type.width + 1);
	mino.pos.col = random_position;
	mino.pos.row = 0;
	return mino;
}
