#include <sys/time.h>
#include <time.h>

#include "ttrs_time.h"
#include "wrapper.h"

static long get_nsec(const struct timespec *ts)
{
	return SEC_TO_NSEC(ts->tv_sec) + ts->tv_nsec;
}

bool is_time_to_fall(t_tetris_time *time)
{
	struct timespec now;

	Clock_gettime(CLOCK_MONOTONIC, &now);
	long now_nsec = get_nsec(&now);
	return now_nsec >= time->next_fall_time;
}

void update_next_fall_time(t_tetris_time *time)
{
	struct timespec now;

	Clock_gettime(CLOCK_MONOTONIC, &now);
	time->next_fall_time = get_nsec(&now) + time->interval;
}

void update_fall_speed(t_tetris_time *time, int count)
{
	if (time->interval < INTERVAL_TIME_MIN) {
		return;
	}
	for (int i = 0; i < count; i++) {
		time->interval -= time->decrease_time;
		if (time->decrease_time > DECREASE_UNIT_TIME)
			time->decrease_time -= DECREASE_UNIT_TIME;
	}
}
