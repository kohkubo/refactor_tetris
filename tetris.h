#ifndef TETRIS
#define TETRIS

#include <stdbool.h>
#include <stdint.h>
#include <sys/time.h>

#include "ttrs_mino.h"
#include "ttrs_time.h"

#define MATRIX_ROW 20
#define MATRIX_COL 15

typedef uint8_t t_matrix[MATRIX_ROW][MATRIX_COL];
typedef uint8_t t_line[MATRIX_ROW];

typedef enum e_status {
	TETRIS_PLAY,
	TETRIS_LOCK_DOWN,
	TETRIS_GAME_OVER,
} t_status;

typedef struct {
	t_matrix matrix;
	size_t clear_line_count;
	t_tetris_time time;
	bool has_to_refresh_screen;
} t_tetris;

#endif
