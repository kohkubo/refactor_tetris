#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/time.h>

#include "ttrs_mino.h"
#include "ttrs_time.h"

#define FIELD_ROW 20
#define FIELD_COL 15

typedef uint8_t t_field[FIELD_ROW][FIELD_COL];
typedef uint8_t t_field_line[FIELD_ROW];

typedef enum e_status {
	TETRIS_FLOATING,
	TETRIS_BOTTOM,
	TETRIS_GAME_OVER,
} t_status;

typedef struct {
	t_field field;
	int64_t score;
	t_tetris_time time;
	bool is_moved;
} t_tetris;

#endif
