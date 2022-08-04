#ifndef TETRIS_HPP
#define TETRIS_HPP

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
	TETRIS_FALL,
	TETRIS_BOTTOM,
	TETRIS_GAME_OVER,
} t_status;

typedef struct {
	t_field field;
	int64_t score;
	t_status status;
	bool is_alive;
	t_tetris_time time;
} t_tetris;

#endif /* TETRIS_HPP */
