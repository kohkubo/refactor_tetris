#include <ncurses.h>
#include <string.h>

#include "ttrs_field.h"
#include "ttrs_print.h"
#include "ttrs_texture.h"

#define GAME_TITLE "42 Tetris\n"

static void print_header()
{
	for (int i = 0; i < FIELD_COL / 2; i++)
		printw(" ");
	printw(GAME_TITLE);
}

void print_field(const t_field_ptr field_ptr, t_print_func print_func)
{
	for (int i = 0; i < FIELD_ROW; i++) {
		for (int j = 0; j < FIELD_COL; j++) {
			char texture = field_ptr[i][j] ? BLOCK_TEXTURE : EMPTY_TEXTURE;
			print_func("%c ", texture);
		}
		print_func("\n");
	}
}

static void print_body(const t_field_ptr field_ptr, t_mino *mino)
{
	t_field_ptr field;

	memcpy(field, field_ptr, sizeof(t_field_ptr));
	place_mino_on_field(field, mino);
	print_field(field, printw);
}

void print_score(int score)
{
	printw("\nScore: %d\n", score);
}

void update_screen(const t_tetris *tetris, t_mino *mino)
{
	clear();
	print_header();
	print_body(tetris->field_ptr, mino);
	print_score(tetris->score);
}
