#include <limits.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "tetris.h"
#include "ttrs_action.h"
#include "ttrs_display.h"
#include "ttrs_matrix.h"
#include "ttrs_mino.h"
#include "ttrs_time.h"
#include "wrapper.h"

extern t_keyhook_func g_keyhooks;

static void destruct_tetris(const t_tetris *tetris)
{
	print_result(tetris);
}

static t_tetris construct_tetris()
{
	t_tetris tetris = {};
	tetris.time.interval = INIT_INTERVAL_TIME;
	tetris.time.decrease_time = INIT_DECREASE_TIME;
	return tetris;
}

static int calculate_score(int clear_line_count)
{
	return SCORE_UNIT * clear_line_count;
}

static t_status handle_locked_down(t_tetris *tetris, t_current_mino *mino)
{
	place_matrix_with_mino(tetris->matrix, mino);
	int clear_line_count = clear_filled_lines(tetris->matrix);
	update_drop_speed(&tetris->time, clear_line_count);
	tetris->score = calculate_score(clear_line_count);
	return try_create_mino(tetris->matrix, mino);
}

static t_status drop_mino_auto(t_tetris *tetris, t_current_mino *mino)
{
	if (is_time_to_drop(&tetris->time)) {
		update_next_drop_time(&tetris->time);
		return try_drop(tetris, mino);
	}
	return TETRIS_PLAY;
}

static void wait_next_frame(long start)
{
	static const long one_frame_usec = SEC_TO_USEC(1) / TTRS_FPS;
	long elapsed = get_current_usec() - start;
	long sleep_time = one_frame_usec - elapsed;

	if (sleep_time > 0)
		usleep(sleep_time);
}

static void run_tetris(t_tetris *tetris)
{
	t_status status = TETRIS_PLAY;
	t_current_mino mino = generate_random_mino();

	while (status != TETRIS_GAME_OVER) {
		long start = get_current_usec();
		refresh_screen(tetris, &mino);
		status = handle_key_input(tetris, &mino);
		if (status == TETRIS_PLAY) {
			status = drop_mino_auto(tetris, &mino);
		}
		if (status == TETRIS_LOCK_DOWN) {
			status = handle_locked_down(tetris, &mino);
		}
		wait_next_frame(start);
	}
}

int main()
{
	t_tetris tetris = construct_tetris();

	srand(time(NULL));
	init_keyhook_func_ptr_array();
	init_ncurses();
	run_tetris(&tetris);
	end_ncurses();
	destruct_tetris(&tetris);
}
