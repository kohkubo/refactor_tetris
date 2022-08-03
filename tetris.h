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

typedef struct {
	t_field field;
	int64_t score;
	bool is_alive;
	t_tetris_time time;
} t_tetris;

typedef bool (*t_keyhook_func)(const t_tetris *, t_mino *);

#endif /* TETRIS_HPP */
