#include "tetris.h"

#include <ncurses.h>

extern t_field g_field;
extern int g_score;

static void print_header()
{
	for (size_t i = 0; i < COL / 2; i++)
		printw(" ");
	printw("42 Tetris\n");
}

static void print_body()
{
	t_field current_field = {};

	update_field(current_field);
	for (size_t i = 0; i < ROW; i++)
	{
		for (size_t j = 0; j < COL; j++)
		{
			printw("%c ", g_field[i][j] || current_field[i][j] ? '#' : '.');
		}
		printw("\n");
	}
}

void print_score()
{
	printw("\nScore: %d\n", g_score);
}

void update_screen()
{
	clear();
	print_header();
	print_body();
	print_score();
}
