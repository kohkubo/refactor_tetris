#ifndef TETRIS_HPP
#define TETRIS_HPP

#define ROW 20
#define COL 15
#define INIT_DECRESE_TIME 1000000
#define INIT_TURN_TIME 400000000

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

typedef uint8_t t_field[ROW][COL];

typedef struct
{
  int row;
  int col;
} t_point;

typedef struct
{
  char **shape;
  size_t width;
} t_mino_shape;

typedef struct
{
  t_mino_shape mino_shape;
  t_point pos;
} t_mino;

typedef bool (*t_keyhook_func)(t_mino *);

t_mino_shape copy_mino_shape(t_mino_shape mino_shape);
t_mino copy_mino(t_mino mino);
void free_mino(t_mino mino);
bool can_place_in_field(t_mino mino, t_point dest);
void rotate_right(t_mino mino);
void update_field(t_field field);
void update_screen();
void print_score();
t_mino generate_random_mino();
bool try_move_down();
bool try_move_left();
bool try_move_right();
bool try_move_rotate();
bool has_to_update();
void update_turn_time(size_t count);
size_t handle_filled_lines();
void run_tetris();
void run_game();

#endif /* TETRIS_HPP */
