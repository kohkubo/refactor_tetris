#include <sys/time.h>
#include <time.h>

#include "ttrs_time.h"
#include "wrapper.h"

static long get_nsec(const struct timespec *ts)
{
	return SEC_TO_NSEC(ts->tv_sec) + ts->tv_nsec;
}

long get_current_nsec()
{
	struct timespec now;

	Clock_gettime(CLOCK_MONOTONIC, &now);
	return get_nsec(&now);
}

long get_current_usec()
{
	return NSEC_TO_USEC(get_current_nsec());
}

bool is_time_to_drop(t_unix_time_nsec next_drop_time)
{
	t_unix_time_nsec now_nsec = get_current_nsec();
	return now_nsec >= next_drop_time;
}

void update_next_drop_time(t_tetris_time *time)
{
	time->next_drop_time = get_current_nsec() + time->interval;
}

void update_drop_speed(t_tetris_time *time, int count)
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
