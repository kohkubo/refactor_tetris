#ifndef TTRS_TIME_H
#define TTRS_TIME_H

#include <stdbool.h>
#include <sys/time.h>
#include <time.h>

#define INIT_DECREASE_TIME 1000000
#define INIT_INTERVAL_TIME 400000000
#define DECREASE_UNIT_TIME 1000
#define INTERVAL_TIME_MIN 1000
#define SEC_TO_NSEC(sec) (sec * 1000000000)

typedef struct
{
	long interval;
	long next_fall_time;
	long decrease_time;
} t_tetris_time;

bool is_time_to_fall(t_tetris_time *time);
void update_fall_speed(t_tetris_time *time, int count);
void update_next_fall_time(t_tetris_time *time);

#endif
