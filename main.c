#include <limits.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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
	g_keyhooks[DOWN_KEY] = try_move_down;
	g_keyhooks[LEFT_KEY] = try_move_left;
	g_keyhooks[RIGHT_KEY] = try_move_right;
	g_keyhooks[ROTATE_KEY] = try_spin;
}

static t_tetris create_tetris()
{
	t_tetris tetris = {
		.field = {},
		.score = 0,
		.is_alive = true,
		.mino = generate_random_mino(),
		.lock = PTHREAD_MUTEX_INITIALIZER,
	};
	tetris.time.interval = INIT_INTERVAL_TIME;
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

// static void start_tetris(t_tetris *tetris)
// {
// 	while (tetris->is_alive) {
// 		handle_key_input(tetris, &tetris->mino);
// 		if (is_time_to_fall(&tetris->time)) {
// 			bool is_reached_ground = !try_move_down(tetris, &tetris->mino);
// 			if (is_reached_ground) {
// 				place_mino_on_field(tetris->field, &tetris->mino);
// 				int num_of_erased = erase_filled_lines(tetris->field);
// 				tetris->mino = generate_random_mino();
// 				update_game_status(tetris, &tetris->mino, num_of_erased);
// 			}
// 			update_screen(tetris, &tetris->mino);
// 			set_next_fall_time(&tetris->time);
// 		}
// 	}
// }

void *start_tetris(void *arg)
{
	t_tetris *tetris = arg;

	while (tetris->is_alive) {
		Pthread_mutex_lock(&tetris->lock);
		bool is_reached_ground = !try_move_down(tetris, &tetris->mino);
		if (is_reached_ground) {
			place_mino_on_field(tetris->field, &tetris->mino);
			int num_of_erased = erase_filled_lines(tetris->field);
			tetris->mino = generate_random_mino();
			update_game_status(tetris, &tetris->mino, num_of_erased);
		}
		update_screen(tetris, &tetris->mino);
		Pthread_mutex_unlock(&tetris->lock);
		usleep(500000);
	}
	return NULL;
}

void *handle_key_input(void *arg)
{
	t_tetris *tetris = arg;

	while (true) {
		int c = getch();
		Pthread_mutex_lock(&tetris->lock);
		if (tetris->is_alive && c != ERR && g_keyhooks[c]) {
			g_keyhooks[c](tetris, &tetris->mino);
			update_screen(tetris, &tetris->mino);
		}
		Pthread_mutex_unlock(&tetris->lock);
	}
	return NULL;
}

void run_key_hooks(t_tetris *tetris)
{
	pthread_t th;

	Pthread_create(&th, NULL, handle_key_input, tetris);
	Pthread_detach(th);
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
	run_key_hooks(tetris);
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
