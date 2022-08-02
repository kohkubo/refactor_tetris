#include "ttrs_mino.h"
#include "tetris.h"

#include <string.h>
#include <stdlib.h>

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

void rotate_right(t_mino_type *mino_type)
{
	t_mino_type temp_shape = *mino_type;
	for (size_t i = 0; i < mino_type->width; i++)
	{
		for (size_t j = 0, k = mino_type->width - 1; j < mino_type->width; j++, k--)
		{
			mino_type->shape[i][j] = temp_shape.shape[k][i];
		}
	}
}

#define ARRAY_SIZE(X) (sizeof(X) / sizeof(X[0]))

t_mino generate_random_mino()
{
	t_mino mino;

	mino.mino_type = g_mino_types[rand() % ARRAY_SIZE(g_mino_types)];
	mino.pos.col = rand() % (FIELD_COL - mino.mino_type.width + 1);
	mino.pos.row = 0;
	return mino;
}
