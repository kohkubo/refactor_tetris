#include "ttrs_time.h"

#include <sys/time.h>
#include <time.h>

static int64_t get_nsec(const struct timespec *ts)
{
	return ts->tv_sec * 1000000000 + ts->tv_nsec;
}

bool is_time_to_fall(t_tetris_time *time)
{
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	int64_t now_nsec = get_nsec(&now);
	int64_t prev_nsec = get_nsec(&time->prev_fall_time);
	int64_t elapsed_time = now_nsec - prev_nsec;
	return elapsed_time > time->interval;
}

void update_fall_speed(t_tetris_time *time, size_t count)
{
	if (time->interval < INTERVAL_TIME_MIN) {
		return;
	}
	for (size_t i = 0; i < count; i++) {
		time->interval -= time->decrease_time;
		if (time->decrease_time > DECREASE_UNIT_TIME)
			time->decrease_time -= DECREASE_UNIT_TIME;
	}
}
