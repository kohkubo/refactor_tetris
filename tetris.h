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
#include <time.h>
#include <sys/time.h>

typedef uint8_t t_field_ptr[ROW][COL];
typedef uint8_t t_field_line_ptr[ROW];

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

typedef struct
{
  t_field_ptr field;
  int score;
  bool game_on;
  long turn_time_nanosec;
} t_game;

typedef bool (*t_keyhook_func)(t_game *, t_mino *);

t_mino_shape copy_mino_shape(const t_mino_shape *mino_shape);
t_mino copy_mino(const t_mino *mino);
void free_mino(t_mino mino);
void update_field(t_field_ptr current_field, t_mino *mino);
void update_screen(t_game *game, t_mino *mino);
bool can_place_in_field(t_field_ptr field, const t_mino_shape *mino_shape, const t_point dest);
void print_score();
t_mino generate_random_mino();
bool try_move_down(t_game *game, t_mino *mino);
bool try_move_left(t_game *game, t_mino *mino);
bool try_move_right(t_game *game, t_mino *mino);
bool try_move_rotate(t_game *game, t_mino *mino);
long turn_time_decrease(size_t count);
bool has_to_update(long turn_time_nanosec);
size_t handle_filled_lines();
void run_tetris();
void run_game();
void rotate_right(t_mino_shape *mino_shape);

#endif /* TETRIS_HPP */
