#ifndef TTRS_PRINT_H
#define TTRS_PRINT_H

#include "tetris.h"

typedef int (*t_print_func)(const char *, ...);

void print_matrix(const t_matrix matrix, t_print_func print_func);
void refresh_screen(t_tetris *tetris, t_mino *mino);
void print_score(size_t clear_line_count, t_print_func print_func);

#endif
