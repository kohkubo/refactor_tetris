#ifndef TTRS_ACTION_H
#define TTRS_ACTION_H

#include "tetris.h"
#include "ttrs_mino.h"

#define DROP_KEY 's'
#define LEFT_KEY 'a'
#define RIGHT_KEY 'd'
#define SPIN_KEY 'w'
#define HARD_DROP_KEY ' '

typedef enum e_status {
	TETRIS_PLAY,
	TETRIS_LOCK_DOWN,
	TETRIS_GAME_OVER,
} t_status;

t_status handle_key_input(t_tetris *tetris, t_current_mino *current);
t_status try_drop(t_tetris *tetris, t_current_mino *current);
t_status try_left(t_tetris *tetris, t_current_mino *current);
t_status try_right(t_tetris *tetris, t_current_mino *current);
t_status try_spin(t_tetris *tetris, t_current_mino *current);
t_status hard_drop(t_tetris *tetris, t_current_mino *current);
t_status try_generate_mino(t_matrix matrix, t_current_mino *current);
void init_keyhook_func_ptr_array(void);
t_status handle_auto_drop(t_tetris *tetris, t_current_mino *current);

typedef t_status (*t_keyhook_func)(t_tetris *tetris, t_current_mino *);

#endif /* TTRS_ACTION_H */
