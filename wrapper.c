#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <time.h>

#include "wrapper.h"

void exit_fatal_err(int code, const char *file, int line)
{
	endwin();
	perror("fatal");
	fprintf(stderr, "fatal error: %s:%d\n", file, line);
	exit(code);
}

void Clock_gettime(clockid_t clk_id, struct timespec *tp)
{
	if (clock_gettime(clk_id, tp) == -1) {
		exit_fatal_err(EXIT_FAILURE, __FILE__, __LINE__);
	}
}

void Puts(const char *str)
{
	if (puts(str) == EOF) {
		exit_fatal_err(EXIT_FAILURE, __FILE__, __LINE__);
	}
}

void Endwin(void)
{
	if (endwin() == ERR) {
		exit_fatal_err(EXIT_FAILURE, __FILE__, __LINE__);
	}
}

void Initscr(void)
{
	if (initscr() == NULL) {
		exit_fatal_err(EXIT_FAILURE, __FILE__, __LINE__);
	}
}
