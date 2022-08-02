#ifndef TTRS_PRINT_H
#define TTRS_PRINT_H

#include "tetris.h"

typedef int (*t_print_func)(const char *, ...);

void print_field(const t_field field, t_print_func print_func);
void update_screen(const t_tetris *tetris, t_mino *mino);
void print_score(int score, t_print_func print_func);

#endif
