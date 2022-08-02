#include "tetris.h"

#include <stdio.h>
#include <stdlib.h>

#include <ncurses.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>
#include <assert.h>
#include <string.h>

t_field g_field = {};
int g_score = 0;
bool g_game_on = true;
long g_turn_time_nanosec = INIT_TURN_TIME;
struct timespec g_time_spec;
t_mino g_current;
t_keyhook_func g_keyhooks[UCHAR_MAX] = {};

static void end_game()
{
	for (size_t i = 0; i < ROW; i++)
	{
		for (size_t j = 0; j < COL; j++)
		{
			printf("%c ", g_field[i][j] ? '#' : '.');
		}
		printf("\n");
	}
	printf("\nGame over!\n");
	print_score();
}

static void init_game()
{
	g_score = 0;
	clock_gettime(CLOCK_MONOTONIC, &g_time_spec);
	g_current = generate_random_mino();
	// assert(can_place_in_field(g_current, g_current.pos));
	g_keyhooks['s'] = try_move_down;
	g_keyhooks['a'] = try_move_left;
	g_keyhooks['d'] = try_move_right;
	g_keyhooks['w'] = try_move_rotate;
}

void reach_bottom()
{
	update_field(g_field);
	size_t count = handle_filled_lines();
	g_score += 100 * count;
	update_turn_time(count);
	free_mino(g_current);
	g_current = generate_random_mino();
	if (!can_place_in_field(g_current, g_current.pos))
	{
		g_game_on = false;
	}
}

void run_tetris()
{
	while (g_game_on)
	{
		int c = getch();
		if (c != ERR && g_keyhooks[c])
		{
			g_keyhooks[c](&g_current);
			update_screen();
		}
		if (has_to_update())
		{
			bool is_moved = try_move_down();
			if (!is_moved)
			{
				reach_bottom();
			}
			update_screen();
			clock_gettime(CLOCK_MONOTONIC, &g_time_spec);
		}
	}
	free_mino(g_current);
}

void run_game()
{
	init_game();
	update_screen();
	run_tetris();
	end_game();
}

int main()
{
	timeout(1);
	srand(time(0));
	initscr();
	run_game();
	endwin();
}
