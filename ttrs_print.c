#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "ttrs_matrix.h"
#include "ttrs_print.h"
#include "wrapper.h"

#define BLOCK_TEXTURE '#'
#define EMPTY_TEXTURE '.'
#define GAME_TITLE "42 Tetris\n"
#define SCORE_UNIT 100

static void print_header()
{
	for (int i = 0; i < MATRIX_COL / 2; i++) {
		if (printw(" ") == ERR) {
			exit_fatal_err(EXIT_FAILURE, __FILE__, __LINE__);
		}
	}
	if (printw(GAME_TITLE) == ERR) {
		exit_fatal_err(EXIT_FAILURE, __FILE__, __LINE__);
	}
}

void print_matrix(const t_matrix matrix, t_print_func print_func)
{
	for (int i = 0; i < MATRIX_ROW; i++) {
		for (int j = 0; j < MATRIX_COL; j++) {
			char texture = matrix[i][j] ? BLOCK_TEXTURE : EMPTY_TEXTURE;
			if (print_func("%c ", texture) == ERR) {
				exit_fatal_err(EXIT_FAILURE, __FILE__, __LINE__);
			}
		}
		if (print_func("\n") == ERR) {
			exit_fatal_err(EXIT_FAILURE, __FILE__, __LINE__);
		}
	}
}

static void print_body(const t_matrix matrix, t_mino *mino)
{
	t_matrix current_matrix;

	memcpy(current_matrix, matrix, sizeof(t_matrix));
	set_matrix_with_mino(current_matrix, mino);
	print_matrix(current_matrix, printw);
}

void print_score(size_t clear_line_count, t_print_func print_func)
{
	if (print_func("\nScore: %zu\n", clear_line_count * SCORE_UNIT) == ERR) {
		exit_fatal_err(EXIT_FAILURE, __FILE__, __LINE__);
	}
}

void refresh_screen(t_tetris *tetris, t_mino *mino)
{
	clear();
	print_header();
	print_body(tetris->matrix, mino);
	print_score(tetris->clear_line_count, printw);
	tetris->has_to_refresh_screen = false;
}
