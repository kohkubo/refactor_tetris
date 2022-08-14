#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "tetris.h"
#include "ttrs_display.h"
#include "ttrs_matrix.h"
#include "wrapper.h"

static void print_header()
{
	for (int i = 0; i < MATRIX_COL / 2; i++) {
		if (printw(" ") == ERR) {
			exit_fatal_err(EXIT_FAILURE);
		}
	}
	if (printw(GAME_TITLE) == ERR) {
		exit_fatal_err(EXIT_FAILURE);
	}
}

void print_matrix(const t_matrix matrix, t_print_func print_func)
{
	for (int i = 0; i < MATRIX_ROW; i++) {
		for (int j = 0; j < MATRIX_COL; j++) {
			char texture = matrix[i][j] ? BLOCK_TEXTURE : EMPTY_TEXTURE;
			if (print_func("%c ", texture) == ERR) {
				exit_fatal_err(EXIT_FAILURE);
			}
		}
		if (print_func("\n") == ERR) {
			exit_fatal_err(EXIT_FAILURE);
		}
	}
}

static void print_body(const t_matrix matrix, t_current_mino *mino)
{
	t_matrix current_matrix;

	memcpy(current_matrix, matrix, sizeof(t_matrix));
	place_matrix_with_mino(current_matrix, mino);
	print_matrix(current_matrix, printw);
}

void print_score(int score, t_print_func print_func)
{
	if (print_func("\n%s%d\n", SCORE_TEXT, score) == ERR) {
		exit_fatal_err(EXIT_FAILURE);
	}
}

void refresh_screen(t_tetris *tetris, t_current_mino *mino)
{
	clear();
	print_header();
	print_body(tetris->matrix, mino);
	print_score(tetris->score, printw);
}

void init_ncurses()
{
	Initscr();
	timeout(1);
	noecho();
	curs_set(0);
}

void end_ncurses()
{
	Endwin();
}
