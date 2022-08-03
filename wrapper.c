#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

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

void Pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
	if (pthread_mutex_init(mutex, attr) == -1) {
		exit_fatal_err(EXIT_FAILURE);
	}
}

void Pthread_mutex_lock(pthread_mutex_t *mutex)
{
	if (pthread_mutex_lock(mutex) == -1) {
		exit_fatal_err(EXIT_FAILURE);
	}
}

void Pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	if (pthread_mutex_unlock(mutex) == -1) {
		exit_fatal_err(EXIT_FAILURE);
	}
}

void Pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
{
	if (pthread_create(thread, attr, start_routine, arg) == -1) {
		exit_fatal_err(EXIT_FAILURE);
	}
}

void Pthread_join(pthread_t thread, void **retval)
{
	if (pthread_join(thread, retval) == -1) {
		exit_fatal_err(EXIT_FAILURE);
	}
}

void Pthread_detach(pthread_t thread)
{
	if (pthread_detach(thread) == -1) {
		exit_fatal_err(EXIT_FAILURE);
	}
}
