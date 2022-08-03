#include <limits.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tetris.h"
#include "ttrs_field.h"
#include "ttrs_keyhook.h"
#include "ttrs_mino.h"
#include "ttrs_print.h"
#include "ttrs_time.h"
#include "wrapper.h"

#define SCORE_UNIT 100

#define DOWN_KEY 's'
#define LEFT_KEY 'a'
#define RIGHT_KEY 'd'
#define ROTATE_KEY 'w'

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
	g_keyhooks[DOWN_KEY] = move_down;
	g_keyhooks[LEFT_KEY] = move_left;
	g_keyhooks[RIGHT_KEY] = move_right;
	g_keyhooks[ROTATE_KEY] = move_spin;
}

static t_tetris create_tetris()
{
	t_tetris tetris = {
		.field = {},
		.score = 0,
		.is_alive = true,
	};
	tetris.time.interval = INIT_INTERVAL_TIME,
	tetris.time.decrease_time = INIT_DECREASE_TIME;
	return tetris;
}

static void update_score(t_tetris *tetris, int num_of_erased)
{
	tetris->score += SCORE_UNIT * num_of_erased;
}

static void update_is_alive(t_tetris *tetris, const t_mino *mino)
{
	tetris->is_alive = can_place_in_field(tetris->field, &mino->mino_type, mino->pos);
}

static void update_game_status(t_tetris *tetris, const t_mino *mino, int num_of_erased)
{
	update_is_alive(tetris, mino);
	update_fall_speed(&tetris->time, num_of_erased);
	update_score(tetris, num_of_erased);
}

static void start_tetris(t_tetris *tetris)
{
	t_mino mino = generate_random_mino();

	while (tetris->is_alive) {
		t_mino moved_mino = handle_key_input(&mino);
		if (can_place_in_field(tetris->field, &moved_mino.mino_type, moved_mino.pos)) {
			mino = moved_mino;
		}

		if (is_time_to_fall(&tetris->time)) {
			moved_mino = move_down(&mino);
			if (can_place_in_field(tetris->field, &moved_mino.mino_type, moved_mino.pos)) {
				mino = moved_mino;
			} else {
				place_mino_on_field(tetris->field, &mino);
				int num_of_erased = erase_filled_lines(tetris->field);
				mino = generate_random_mino();
				update_game_status(tetris, &mino, num_of_erased);
			}
			set_next_fall_time(&tetris->time);
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
	start_tetris(tetris);
	Endwin();
}

int main()
{
	t_tetris tetris = create_tetris();
	init_tetris();
	run_tetris(&tetris);
	end_tetris(&tetris);
}
