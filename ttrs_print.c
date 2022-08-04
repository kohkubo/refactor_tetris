#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "ttrs_field.h"
#include "ttrs_print.h"
#include "wrapper.h"

#define BLOCK_TEXTURE '#'
#define EMPTY_TEXTURE '.'
#define GAME_TITLE "42 Tetris\n"

static void print_header()
{
	for (int i = 0; i < FIELD_COL / 2; i++) {
		if (printw(" ") == ERR) {
			exit_fatal_err(EXIT_FAILURE);
		}
	}
	if (printw(GAME_TITLE) == ERR) {
		exit_fatal_err(EXIT_FAILURE);
	}
}

void print_field(const t_field field, t_print_func print_func)
{
	for (int i = 0; i < FIELD_ROW; i++) {
		for (int j = 0; j < FIELD_COL; j++) {
			char texture = field[i][j] ? BLOCK_TEXTURE : EMPTY_TEXTURE;
			if (print_func("%c ", texture) == ERR) {
				exit_fatal_err(EXIT_FAILURE);
			}
		}
		if (print_func("\n") == ERR) {
			exit_fatal_err(EXIT_FAILURE);
		}
	}
}

static void print_body(const t_field field, t_mino *mino)
{
	t_field current_field;

	memcpy(current_field, field, sizeof(t_field));
	place_mino_on_field(current_field, mino);
	print_field(current_field, printw);
}

void print_score(int score, t_print_func print_func)
{
	if (print_func("\nScore: %d\n", score) == ERR) {
		exit_fatal_err(EXIT_FAILURE);
	}
}

void refresh_screen(const t_tetris *tetris, t_mino *mino)
{
	clear();
	print_header();
	print_body(tetris->field, mino);
	print_score(tetris->score, printw);
}
