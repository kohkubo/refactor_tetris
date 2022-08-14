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

static t_tetris init_tetris()
{
	t_tetris tetris = {};
	tetris.time.interval = INIT_INTERVAL_TIME;
	tetris.time.decrease_time = INIT_DECREASE_TIME;
	tetris.current_mino = generate_random_mino();
	return tetris;
}

static int calculate_score(int num_of_cleared_lines)
{
	return SCORE_UNIT * num_of_cleared_lines;
}

static t_is_gamover exec_one_frame(t_tetris *tetris)
{
	t_is_gamover is_gameover = false;
	t_status status = handle_key_input(tetris, &tetris->current_mino);

	if (status == TETRIS_PLAY) {
		status = handle_auto_drop(tetris, &tetris->current_mino);
	}
	if (status == TETRIS_LOCK_DOWN) {
		const int num_of_cleared_lines = lock_down_current_mino(tetris);
		tetris->score += calculate_score(num_of_cleared_lines);
		tetris->current_mino = generate_random_mino();
		is_gameover = !can_place_on_matrix(tetris->matrix, &tetris->current_mino.mino_type, tetris->current_mino.pos);
	}
	return is_gameover;
}

static void wait_next_frame(t_unix_time_usec start)
{
	static const t_unix_time_usec one_frame = SEC_TO_USEC(1) / TTRS_FPS;
	t_unix_time_usec elapsed = get_current_usec() - start;
	t_unix_time_usec sleep_time = one_frame - elapsed;

	if (sleep_time > 0)
		usleep(sleep_time);
}

static void run_tetris_loop(t_tetris *tetris)
{
	t_is_gamover is_gameover = false;

	while (!is_gameover) {
		t_unix_time_usec start = get_current_usec();
		is_gameover = exec_one_frame(tetris);
		refresh_screen(tetris);
		wait_next_frame(start);
	}
}

static void init_game()
{
	srand(time(NULL));
	init_keyhook_func_ptr_array();
}

int main()
{
	init_game();
	t_tetris tetris = init_tetris();

	init_ncurses();
	run_tetris_loop(&tetris);
	end_ncurses();

	print_tetris_result(&tetris);
}
