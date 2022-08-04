#ifndef TTRS_PRINT_H
#define TTRS_PRINT_H

#include "tetris.h"

typedef int (*t_print_func)(const char *, ...);

void print_matrix(const t_matrix matrix, t_print_func print_func);
void refresh_screen(const t_tetris *tetris, t_mino *mino);
void print_score(int score, t_print_func print_func);

#endif
