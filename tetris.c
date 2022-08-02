#include "tetris.h"

#include <ncurses.h>

t_keyhook_func g_keyhooks[UCHAR_MAX] = {};

static void end_tetris(const t_tetris *tetris)
{
	for (size_t i = 0; i < FIELD_ROW; i++)
	{
		for (size_t j = 0; j < FIELD_COL; j++)
		{
			printf("%c ", tetris->field_ptr[i][j] ? '#' : '.');
		}
		putchar('\n');
	}
	puts("\nGame over!");
	print_score(tetris->score);
}

static void assign_keyhook_funcp()
{
	g_keyhooks['s'] = try_move_down;
	g_keyhooks['a'] = try_move_left;
	g_keyhooks['d'] = try_move_right;
	g_keyhooks['w'] = try_move_rotate;
}

static t_tetris create_tetris()
{
	t_tetris tetris = {
		.field_ptr = {},
		.score = 0,
		.is_alive = true,
	};
	tetris.time.interval = INIT_INTERVAL_TIME,
	clock_gettime(CLOCK_MONOTONIC, &tetris.time.prev_fall_time);
	tetris.time.decrease_time = INIT_DECREASE_TIME;
	return tetris;
}

static void start_tetris(t_tetris *tetris)
{
	t_mino mino = generate_random_mino();

	while (tetris->is_alive)
	{
		handle_key_input(tetris, &mino);
		if (is_time_to_fall(&tetris->time))
		{
			bool is_reached_ground = try_move_down(tetris, &mino) == false;
			if (is_reached_ground)
			{
				place_mino_on_field(tetris->field_ptr, &mino);
				size_t num_of_erased = erase_filled_lines(tetris->field_ptr);
				update_fall_speed(&tetris->time, num_of_erased);
				mino = generate_random_mino();
				tetris->is_alive = can_place_in_field(tetris->field_ptr, &mino.mino_type, mino.pos);
				tetris->score += SCORE_UNIT * num_of_erased;
			}
			update_screen(tetris, &mino);
			clock_gettime(CLOCK_MONOTONIC, &tetris->time.prev_fall_time);
		}
	}
}

static void init_tetris()
{
	assign_keyhook_funcp();
	timeout(1);
}

static void run_game()
{
	init_tetris();
	t_tetris tetris = create_tetris();
	start_tetris(&tetris);
	end_tetris(&tetris);
}

int main()
{
	srand(time(NULL));
	initscr();
	run_game();
	endwin();
}
