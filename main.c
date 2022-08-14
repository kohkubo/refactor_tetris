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
typedef bool t_is_gamover;

static void end_tetris(const t_tetris *tetris)
{
	print_result(tetris);
}

static t_tetris init_tetris()
{
	t_tetris tetris = {};
	tetris.time.interval = INIT_INTERVAL_TIME;
	tetris.time.decrease_time = INIT_DECREASE_TIME;
	tetris.current_mino = generate_random_mino();
	return tetris;
}

static int calculate_score(int clear_line_count)
{
	return SCORE_UNIT * clear_line_count;
}

static void locked_down_current_mino(t_tetris *tetris, t_current_mino *mino)
{
	place_matrix_with_mino(tetris->matrix, mino);
	int clear_line_count = clear_filled_lines(tetris->matrix);
	update_drop_speed(&tetris->time, clear_line_count);
	tetris->score = calculate_score(clear_line_count);
}

static t_status handle_auto_drop(t_tetris *tetris, t_current_mino *mino)
{
	if (is_time_to_drop(&tetris->time)) {
		update_next_drop_time(&tetris->time);
		return try_drop(tetris, mino);
	}
	return TETRIS_PLAY;
}

static t_is_gamover update_tetris(t_tetris *tetris)
{
	t_status status = handle_key_input(tetris, &tetris->current_mino);
	if (status == TETRIS_PLAY) {
		status = handle_auto_drop(tetris, &tetris->current_mino);
	}
	if (status == TETRIS_LOCK_DOWN) {
		locked_down_current_mino(tetris, &tetris->current_mino);
		status = try_create_mino(tetris->matrix, &tetris->current_mino);
	}
	return status == TETRIS_GAME_OVER;
}

static void wait_next_frame(t_unix_time_usec start)
{
	static const t_unix_time_usec one_frame = SEC_TO_USEC(1) / TTRS_FPS;
	t_unix_time_usec elapsed = get_current_usec() - start;
	t_unix_time_usec sleep_time = one_frame - elapsed;

	if (sleep_time > 0)
		usleep(sleep_time);
}

static void run_game_loop(t_tetris *tetris)
{
	t_is_gamover is_gameover = false;

	while (!is_gameover) {
		t_unix_time_usec start = get_current_usec();
		refresh_screen(tetris);
		is_gameover = update_tetris(tetris);
		wait_next_frame(start);
	}
}

int main()
{
	srand(time(NULL));
	init_keyhook_func_ptr_array();
	init_ncurses();
	t_tetris tetris = init_tetris();
	run_game_loop(&tetris);
	end_ncurses();
	end_tetris(&tetris);
}
