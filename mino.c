#include "tetris.h"

#include <string.h>

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

void free_mino(t_mino **mino)
{
	free(*mino);
	*mino = NULL;
}

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

t_mino *generate_random_mino()
{
	t_mino *mino = (t_mino *)malloc(sizeof(t_mino));
	mino->mino_type = g_mino_types[rand() % (sizeof(g_mino_types) / sizeof(t_mino_type))];
	mino->pos.col = rand() % (FIELD_COL - mino->mino_type.width + 1);
	mino->pos.row = 0;
	return mino;
}
