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

int Clock_gettime(clockid_t clk_id, struct timespec *tp)
{
	int ret = clock_gettime(clk_id, tp);
	if (ret == -1) {
		exit_fatal_err(EXIT_FAILURE, __FILE__, __LINE__);
	}
	return ret;
}

int Puts(const char *str)
{
	int ret = puts(str);
	if (ret == EOF) {
		exit_fatal_err(EXIT_FAILURE, __FILE__, __LINE__);
	}
	return ret;
}

int Endwin(void)
{
	int ret = endwin();
	if (ret == ERR) {
		exit_fatal_err(EXIT_FAILURE, __FILE__, __LINE__);
	}
	return ret;
}

WINDOW *Initscr(void)
{
	WINDOW *ret = initscr();
	if (ret == NULL) {
		exit_fatal_err(EXIT_FAILURE, __FILE__, __LINE__);
	}
	return ret;
}

int Clear(void)
{
	int ret = clear();
	if (ret == ERR) {
		exit_fatal_err(EXIT_FAILURE, __FILE__, __LINE__);
	}
	return ret;
}
