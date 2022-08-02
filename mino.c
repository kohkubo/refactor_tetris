#include "tetris.h"

#include <string.h>

// clang-format off
const t_mino_shape g_mino_shapes[] = {
	{
		.shape = (char *[]){
			(char[]){0, 1, 1},
			(char[]){1, 1, 0},
			(char[]){0, 0, 0}
		},
		.width = 3
	},
	{
		.shape = (char *[]){
			(char[]){1, 1, 0},
			(char[]){0, 1, 1},
			(char[]){0, 0, 0}
		},
		.width = 3},
	{
		.shape = (char *[]){
			(char[]){0, 1, 0},
			(char[]){1, 1, 1},
			(char[]){0, 0, 0}
		},
		.width = 3
	},
	{
		.shape = (char *[]){
			(char[]){0, 0, 1},
			(char[]){1, 1, 1},
			(char[]){0, 0, 0}
		},
		.width = 3
	},
	{
		.shape = (char *[]){
			(char[]){1, 0, 0},
			(char[]){1, 1, 1},
			(char[]){0, 0, 0}
		},
	 	.width = 3
	},
	{
		.shape = (char *[]){
			(char[]){1, 1},
			(char[]){1, 1}
		},
		.width = 2
	},
	{
		.shape = (char *[]){
			(char[]){0, 0, 0, 0},
			(char[]){1, 1, 1, 1},
			(char[]){0, 0, 0, 0},
			(char[]){0, 0, 0, 0}
		},
		.width = 4
	}
};
// clang-format on

t_mino_shape copy_mino_shape(const t_mino_shape *mino_shape)
{
	t_mino_shape new_mino_shape;
	new_mino_shape.width = mino_shape->width;
	new_mino_shape.shape = (char **)malloc(sizeof(char *) * mino_shape->width);
	for (size_t i = 0; i < mino_shape->width; i++)
	{
		new_mino_shape.shape[i] = (char *)malloc(sizeof(char) * mino_shape->width);
		memcpy(new_mino_shape.shape[i], mino_shape->shape[i], mino_shape->width);
	}
	return new_mino_shape;
}

t_mino copy_mino(const t_mino *mino)
{
	t_mino new_shape;

	new_shape.pos = mino->pos;
	new_shape.mino_shape = copy_mino_shape(&mino->mino_shape);
	return new_shape;
}

void free_mino_shape(t_mino_shape *mino_shape)
{
	for (size_t i = 0; i < mino_shape->width; i++)
	{
		free(mino_shape->shape[i]);
	}
	free(mino_shape->shape);
}

void free_mino(t_mino mino)
{
	for (size_t i = 0; i < mino.mino_shape.width; i++)
	{
		free(mino.mino_shape.shape[i]);
	}
	free(mino.mino_shape.shape);
}

void rotate_right(t_mino_shape *mino_shape)
{
	t_mino_shape temp_shape = copy_mino_shape(mino_shape);
	for (size_t i = 0; i < mino_shape->width; i++)
	{
		for (size_t j = 0, k = mino_shape->width - 1; j < mino_shape->width; j++, k--)
		{
			mino_shape->shape[i][j] = temp_shape.shape[k][i];
		}
	}
	free_mino_shape(&temp_shape);
}

t_mino generate_random_mino()
{
	t_mino new_mino;
	new_mino.mino_shape = copy_mino_shape(&g_mino_shapes[rand() % sizeof(g_mino_shapes) / sizeof(t_mino_shape)]);
	new_mino.pos.col = rand() % (COL - new_mino.mino_shape.width + 1);
	new_mino.pos.row = 0;
	return new_mino;
}
