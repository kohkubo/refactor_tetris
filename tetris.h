#ifndef TETRIS_HPP
#define TETRIS_HPP

#define FIELD_ROW 20
#define FIELD_COL 15
#define SHAPE_HEIGHT_MAX 4
#define SHAPE_WIDTH_MAX 4
#define INIT_DECREASE_TIME 1000000
#define INIT_INTERVAL_TIME 400000000
#define DECREASE_UNIT_TIME 1000
#define INTERVAL_TIME_MIN 1000
#define SCORE_UNIT 100

#define BLOCK_TEXTURE '#'
#define EMPTY_TEXTURE '.'

#define DOWN_KEY 's'
#define LEFT_KEY 'a'
#define RIGHT_KEY 'd'
#define ROTATE_KEY 'w'

#define GAME_TITLE "42 Tetris\n"
#define GAME_OVER "\nGame over!"

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

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
  long interval;
  struct timespec prev_fall_time;
  long decrease_time;
} t_tetris_time;

typedef struct
{
  t_field_ptr field_ptr;
  int64_t score;
  bool is_alive;
  t_tetris_time time;
} t_tetris;

typedef bool (*t_keyhook_func)(const t_tetris *, t_mino *);

void free_mino(t_mino **mino);

void place_mino_on_field(t_field_ptr current_field, t_mino *mino);

void update_screen(const t_tetris *tetris, t_mino *mino);
bool can_place_in_field(const t_field_ptr field_ptr,
                        const t_mino_type *mino_type, const t_point dest);
void print_score(int score);
void handle_key_input(const t_tetris *tetris, t_mino *mino);
size_t erase_filled_lines(t_field_ptr field_ptr);

bool try_move_down(const t_tetris *tetris, t_mino *mino);
bool try_move_left(const t_tetris *tetris, t_mino *mino);
bool try_move_right(const t_tetris *tetris, t_mino *mino);
bool try_move_rotate(const t_tetris *tetris, t_mino *mino);

void update_fall_speed(t_tetris_time *time, size_t count);
t_mino generate_random_mino();
bool is_time_to_fall(t_tetris_time *time);

void rotate_right(t_mino_type *mino_type);

#endif /* TETRIS_HPP */
