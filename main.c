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

t_mino_move_func g_move_mino[UCHAR_MAX] = {};

static void end_tetris(const t_tetris *tetris)
{
	print_field(tetris->field, printf);
	Puts(GAME_OVER);
	print_score(tetris->score, printf);
}

static void assign_keyhook_funcp()
{
	g_move_mino[DOWN_KEY] = move_down;
	g_move_mino[LEFT_KEY] = move_left;
	g_move_mino[RIGHT_KEY] = move_right;
	g_move_mino[ROTATE_KEY] = move_spin;
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

static void update_is_alive(t_tetris *tetris, const t_mino *mino)
{
	tetris->is_alive = can_place_in_field(tetris->field, &mino->mino_type, mino->pos.row, mino->pos.col);
}

static void update_game_status(t_tetris *tetris, const t_mino *mino, int num_of_erased)
{
	update_is_alive(tetris, mino);
	update_fall_speed(&tetris->time, num_of_erased);
	update_score(tetris, num_of_erased);
}

bool is_reached_bottom(t_tetris *tetris, const t_mino *mino)
{
	t_mino moved_mino = g_move_mino[DOWN_KEY](mino);
	return !can_place_in_field(tetris->field, &moved_mino.mino_type, moved_mino.pos.row, moved_mino.pos.col);
}

void handle_reached_bottom(t_tetris *tetris, t_mino *mino)
{
	place_mino_on_field(tetris->field, mino);
	int num_of_erased = erase_filled_lines(tetris->field);
	*mino = generate_random_mino();
	update_game_status(tetris, mino, num_of_erased);
}

bool try_move_mino(t_tetris *tetris, t_mino *mino, uint8_t command)
{
	t_mino moved_mino = g_move_mino[command](mino);
	if (can_place_in_field(tetris->field, &moved_mino.mino_type, moved_mino.pos.row, moved_mino.pos.col)) {
		*mino = moved_mino;
		return true;
	}
	return false;
}

bool is_valid_key(int c)
{
	return g_move_mino[c] != NULL;
}

int try_move_mino_by_key_input(t_tetris *tetris, t_mino *mino)
{
	int key = getch();

	if (key == ERR || !is_valid_key(key)) {
		return false;
	}
	return try_move_mino(tetris, mino, key);
}

bool handle_auto_fall(t_tetris *tetris, t_mino *mino)
{
	if (is_time_to_fall(&tetris->time)) {
		set_next_fall_time(&tetris->time);
		return try_move_mino(tetris, mino, DOWN_KEY);
	}
	return false;
}

static void loop_tetris(t_tetris *tetris)
{
	t_mino mino = generate_random_mino();

	while (tetris->is_alive) {
		bool is_moved = try_move_mino_by_key_input(tetris, &mino);
		is_moved |= handle_auto_fall(tetris, &mino);
		if (is_moved)
			update_screen(tetris, &mino);
		if (is_moved && is_reached_bottom(tetris, &mino)) {
			handle_reached_bottom(tetris, &mino);
			update_screen(tetris, &mino);
		}
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
