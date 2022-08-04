#ifndef WRAPPER_H
#define WRAPPER_H

#include <curses.h>
#include <time.h>

void exit_fatal_err(int code);
int Clock_gettime(clockid_t clk_id, struct timespec *tp);
int Puts(const char *str);
int Endwin(void);
WINDOW *Initscr(void);
int Clear(void);

#endif
