#ifndef TTRS_MINO_H
#define TTRS_MINO_H

#define SHAPE_HEIGHT_MAX 4
#define SHAPE_WIDTH_MAX 4

#include <stddef.h>

typedef struct
{
	int row;
	int col;
} t_point;

typedef char t_shape[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];

typedef struct
{
	t_shape shape;
	int width;
} t_mino_type;

typedef struct
{
	t_mino_type mino_type;
	t_point pos;
} t_current_mino;

t_current_mino generate_random_mino();
void spin_right(t_mino_type *mino_type);

#endif
