#ifndef WRAPPER_H
#define WRAPPER_H

#include <pthread.h>
#include <time.h>

void exit_fatal_err(int code);
void Clock_gettime(clockid_t clk_id, struct timespec *tp);
void Puts(const char *str);
void Endwin(void);
void Initscr(void);

void Pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
void Pthread_mutex_lock(pthread_mutex_t *mutex);
void Pthread_mutex_unlock(pthread_mutex_t *mutex);
void Pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
void Pthread_join(pthread_t thread, void **retval);
void Pthread_detach(pthread_t thread);

#endif
