#include <stdlib.h>
#include <string.h>

#include "tetris.h"
#include "ttrs_mino.h"

// clang-format off
const t_mino_type g_mino_types[] = {
	{
		.shape = {
			{0, 1, 1},
			{1, 1, 0}
		},
		.width = 3,
	},
	{
		.shape = {
			{1, 1, 0},
			{0, 1, 1},
		},
		.width = 3
	},
	{
		.shape = {
			{0, 1, 0},
			{1, 1, 1},
		},
		.width = 3
	},
	{
		.shape = {
			{0, 0, 1},
			{1, 1, 1},
		},
		.width = 3
	},
	{
		.shape = {
			{1, 0, 0},
			{1, 1, 1},
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
			{1, 1, 1, 1},
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

#define ARRAY_SIZE(X) (sizeof(X) / sizeof(X[0]))

t_mino generate_random_mino()
{
	t_mino mino;

	mino.mino_type = g_mino_types[rand() % ARRAY_SIZE(g_mino_types)];
	mino.pos.col = rand() % (MATRIX_COL - mino.mino_type.width + 1);
	mino.pos.row = 0;
	return mino;
}
