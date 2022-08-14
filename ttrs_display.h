#ifndef TTRS_PRINT_H
#define TTRS_PRINT_H

#include "tetris.h"

#define BLOCK_TEXTURE '#'
#define EMPTY_TEXTURE '.'

#define GAME_TITLE "42 Tetris\n"
#define SCORE_TEXT "Score: "
#define GAME_OVER_TEXT "\nGame over!"

typedef int (*t_print_func)(const char *, ...);

void refresh_screen(t_tetris *tetris, t_current_mino *mino);
void print_result(const t_tetris *tetris);
void init_ncurses();
void end_ncurses();

#endif
