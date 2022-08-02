#ifndef TETRIS_HPP
#define TETRIS_HPP

#define FIELD_ROW 20
#define FIELD_COL 15
#define SHAPE_HEIGHT_MAX 4
#define SHAPE_WIDTH_MAX 4
#define INIT_DECRESE_TIME 1000000
#define INIT_TURN_TIME 400000000

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <sys/time.h>

typedef uint8_t t_field_ptr[FIELD_ROW][FIELD_COL];
typedef uint8_t t_field_line_ptr[FIELD_ROW];

typedef struct
{
  int row;
  int col;
} t_point;

typedef struct
{
  char shape[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
  size_t width;
} t_mino_type;

typedef struct
{
  t_mino_type mino_type;
  t_point pos;
} t_mino;

typedef struct
{
  t_field_ptr field_ptr;
  int score;
  bool game_on;
  long turn_time_nanosec;
} t_game;

typedef bool (*t_keyhook_func)(t_game *, t_mino *);

void free_mino(t_mino **mino);

void update_field(t_field_ptr current_field, t_mino *mino);

void update_screen(t_game *game, t_mino *mino);
bool can_place_in_field(t_field_ptr field_ptr, const t_mino_type *mino_type, const t_point dest);
void print_score(int score);
void handle_key_input(t_game *game, t_mino *mino);
size_t erase_filled_lines(t_field_ptr field_ptr);

bool try_move_down(t_game *game, t_mino *mino);
bool try_move_left(t_game *game, t_mino *mino);
bool try_move_right(t_game *game, t_mino *mino);
bool try_move_rotate(t_game *game, t_mino *mino);

long turn_time_decrease(size_t count);
bool is_update_time(long turn_time_nanosec);
t_mino *generate_random_mino();

void rotate_right(t_mino_type *mino_type);

#endif /* TETRIS_HPP */
