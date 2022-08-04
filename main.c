#include <limits.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tetris.h"
#include "ttrs_action.h"
#include "ttrs_field.h"
#include "ttrs_mino.h"
#include "ttrs_print.h"
#include "ttrs_time.h"
#include "wrapper.h"

#define SCORE_UNIT 100

#define GAME_OVER "\nGame over!"

t_keyhook_func g_keyhooks[UCHAR_MAX] = {};

static void end_tetris(const t_tetris *tetris)
{
	print_field(tetris->field, printf);
	Puts(GAME_OVER);
	print_score(tetris->score, printf);
}

static void assign_keyhook_funcp()
{
	g_keyhooks[DOWN_KEY] = try_down;
	g_keyhooks[LEFT_KEY] = try_left;
	g_keyhooks[RIGHT_KEY] = try_right;
	g_keyhooks[ROTATE_KEY] = try_spin;
	g_keyhooks[SPACE_KEY] = down_direction;
}

static t_tetris create_tetris()
{
	t_tetris tetris = {
		.field = {},
		.score = 0,
	};
	tetris.time.interval = INIT_INTERVAL_TIME,
	tetris.time.decrease_time = INIT_DECREASE_TIME;
	return tetris;
}

static void update_score(t_tetris *tetris, int num_of_erased)
{
	tetris->score += SCORE_UNIT * num_of_erased;
}

static t_status update_game(t_tetris *tetris, t_mino *mino, int num_of_erased)
{
	update_fall_speed(&tetris->time, num_of_erased);
	update_score(tetris, num_of_erased);
	return create_new_mino(tetris->field, mino);
}

static t_status reached_bottom(t_tetris *tetris, t_mino *mino)
{
	place_mino_on_field(tetris->field, mino);
	int num_of_erased = erase_filled_lines(tetris->field);
	return update_game(tetris, mino, num_of_erased);
}

static t_status fall(t_tetris *tetris, t_mino *mino)
{
	if (is_time_to_fall(&tetris->time)) {
		set_next_fall_time(&tetris->time);
		return try_down(tetris, mino);
	}
	return TETRIS_FALL;
}

static void loop_tetris(t_tetris *tetris)
{
	t_status status = TETRIS_FALL;
	t_mino mino = generate_random_mino();
	while (true) {
		status = handle_key_input(tetris, &mino);
		if (status == TETRIS_FALL) {
			status = fall(tetris, &mino);
		}
		if (status == TETRIS_BOTTOM) {
			status = reached_bottom(tetris, &mino);
		}
		if (status == TETRIS_GAME_OVER) {
			return;
		}
		update_screen(tetris, &mino);
	}
}

static void init_tetris()
{
	srand(time(NULL));
	assign_keyhook_funcp();
}

static void run_tetris(t_tetris *tetris)
{
	Initscr();
	timeout(1);
	loop_tetris(tetris);
	Endwin();
}

int main()
{
	t_tetris tetris = create_tetris();
	init_tetris();
	run_tetris(&tetris);
	end_tetris(&tetris);
}
