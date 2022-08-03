#ifndef WRAPPER_H
#define WRAPPER_H

#include <time.h>

void exit_fatal_err(int code);
void Clock_gettime(clockid_t clk_id, struct timespec *tp);
void Puts(const char *str);
void Endwin(void);
void Initscr(void);

#endif
