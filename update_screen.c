#include "tetris.h"

#include <ncurses.h>

extern t_field_ptr g_field;
extern int g_score;

static void print_header()
{
	for (size_t i = 0; i < COL / 2; i++)
		printw(" ");
	printw("42 Tetris\n");
}

static void print_body(t_game *game, t_mino *mino)
{
	t_field_ptr current_field = {};

	update_field(current_field, mino);
	for (size_t i = 0; i < ROW; i++)
	{
		for (size_t j = 0; j < COL; j++)
		{
			printw("%c ", game->field[i][j] || current_field[i][j] ? '#' : '.');
		}
		printw("\n");
	}
}

void print_score(int score)
{
	printw("\nScore: %d\n", score);
}

void update_screen(t_game *game, t_mino *mino)
{
	clear();
	print_header();
	print_body(game, mino);
	print_score(game->score);
}
