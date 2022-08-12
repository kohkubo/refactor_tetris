#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/time.h>

#include "ttrs_mino.h"
#include "ttrs_time.h"

#define MATRIX_ROW 20
#define MATRIX_COL 15

#define SCORE_UNIT 100

typedef uint8_t t_matrix[MATRIX_ROW][MATRIX_COL];
typedef uint8_t t_line[MATRIX_ROW];

typedef struct {
	t_matrix matrix;
	size_t clear_line_count;
	t_tetris_time time;
} t_tetris;

#endif /* TETRIS_H */
