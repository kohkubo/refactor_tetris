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

struct timespec g_time_spec;
t_keyhook_func g_keyhooks[UCHAR_MAX] = {};

int fd;

static void end_game(const t_game *game)
{
	for (size_t i = 0; i < ROW; i++)
	{
		for (size_t j = 0; j < COL; j++)
		{
			printf("%c ", game->field[i][j] ? '#' : '.');
		}
		putchar('\n');
	}
	puts("\nGame over!");
	print_score();
}

static void assign_keyhook_funcp()
{
	g_keyhooks['s'] = try_move_down;
	g_keyhooks['a'] = try_move_left;
	g_keyhooks['d'] = try_move_right;
	g_keyhooks['w'] = try_move_rotate;
}

static void init_all()
{
	srand(time(0));
	assign_keyhook_funcp();
	initscr();
	timeout(1);
	clock_gettime(CLOCK_MONOTONIC, &g_time_spec);
}

static t_game create_game()
{
	t_game game = {
		.field = {},
		.score = 0,
		.game_on = true,
		.turn_time_nanosec = INIT_TURN_TIME,
	};
	return game;
}

void reach_bottom(t_game *game, t_mino *mino)
{
	update_field(game->field, mino);

	size_t count = handle_filled_lines(&game->field);

	game->score += 100 * count;
	game->turn_time_nanosec -= turn_time_decrease(count);

	free_mino(*mino);
	*mino = generate_random_mino();

	game->game_on = can_place_in_field(game->field, &mino->mino_shape, mino->pos);
}

static void handle_key_input(t_game *game, t_mino *mino)
{
	int c = getch();
	if (c != ERR && g_keyhooks[c])
	{
		g_keyhooks[c](game, mino);
	}
}

void run_tetris(t_game *game)
{
	t_mino mino = generate_random_mino();
	while (game->game_on)
	{
		update_screen(game, &mino);
		handle_key_input(game, &mino);
		if (!has_to_update(game->turn_time_nanosec))
		{
			continue;
		}
		bool is_reached_bottom = try_move_down(game, &mino) == false;
		if (is_reached_bottom)
		{
			reach_bottom(game, &mino);
		}
		clock_gettime(CLOCK_MONOTONIC, &g_time_spec);
	}
	free_mino(mino);
}

void run_game()
{
	t_game game = create_game();
	run_tetris(&game);
	end_game(&game);
}

int main()
{
	init_all();
	run_game();
	endwin();
}
