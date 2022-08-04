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

extern t_keyhook_func g_keyhooks[UCHAR_MAX];

static void end_tetris(const t_tetris *tetris)
{
	print_field(tetris->field, printf);
	Puts(GAME_OVER);
	print_score(tetris->score, printf);
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

static void update_score(t_tetris *tetris, int erased_count)
{
	tetris->score += SCORE_UNIT * erased_count;
}

static t_status update_game(t_tetris *tetris, t_mino *mino, int erased_count)
{
	update_fall_speed(&tetris->time, erased_count);
	update_score(tetris, erased_count);
	return create_new_mino(tetris->field, mino);
}

static t_status reached_bottom(t_tetris *tetris, t_mino *mino)
{
	place_mino_on_field(tetris->field, mino);
	int erased_count = erase_filled_lines(tetris->field);
	return update_game(tetris, mino, erased_count);
}

static t_status fall(t_tetris *tetris, t_mino *mino)
{
	if (is_time_to_fall(&tetris->time)) {
		update_next_fall_time(&tetris->time);
		return try_down(tetris, mino);
	}
	return TETRIS_FLOATING;
}

static void run_tetris(t_tetris *tetris)
{
	t_status status = TETRIS_FLOATING;
	t_mino mino = generate_random_mino();
	while (true) {
		status = handle_key_input(tetris, &mino);
		if (status == TETRIS_FLOATING) {
			status = fall(tetris, &mino);
		}
		if (tetris->is_moved) {
			refresh_screen(tetris, &mino);
		}
		if (status == TETRIS_BOTTOM) {
			status = reached_bottom(tetris, &mino);
		}
		if (status == TETRIS_GAME_OVER) {
			return;
		}
	}
}

static void init_game()
{
	Initscr();
	timeout(1);
	srand(time(NULL));
	assign_keyhook_funcp();
}

int main()
{
	init_game();
	t_tetris tetris = create_tetris();
	run_tetris(&tetris);
	Endwin();
	end_tetris(&tetris);
}
