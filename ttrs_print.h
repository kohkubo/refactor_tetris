#ifndef TTRS_PRINT_HPP
#define TTRS_PRINT_HPP

#include "tetris.h"
typedef int (*t_print_func)(const char *, ...);

void print_field(const t_field_ptr field_ptr, t_print_func print_func);
void update_screen(const t_tetris *tetris, t_mino *mino);
void print_score(int score);

#endif /* TTRS_PRINT_HPP */
