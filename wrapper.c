#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "wrapper.h"

void exit_fatal_err(int code)
{
	endwin();
	perror("fatal");
	exit(code);
}

void Clock_gettime(clockid_t clk_id, struct timespec *tp)
{
	if (clock_gettime(clk_id, tp) == -1) {
		exit_fatal_err(EXIT_FAILURE);
	}
}

void Puts(const char *str)
{
	if (puts(str) == EOF) {
		exit_fatal_err(EXIT_FAILURE);
	}
}

void Endwin(void)
{
	if (endwin() == ERR) {
		exit_fatal_err(EXIT_FAILURE);
	}
}

void Initscr(void)
{
	if (initscr() == NULL) {
		exit_fatal_err(EXIT_FAILURE);
	}
}
