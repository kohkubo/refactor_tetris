#include "tetris.h"

#include <time.h>
#include <sys/time.h>

extern long g_turn_time_nanosec;
extern struct timespec g_time_spec;

static int64_t get_nsec(const struct timespec *ts)
{
	return ts->tv_sec * 1000000000 + ts->tv_nsec;
}

bool is_update_time(long interval_nanosec)
{
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	int64_t now_nsec = get_nsec(&now);
	int64_t prev_nsec = get_nsec(&g_time_spec);
	int64_t elapsed_time = now_nsec - prev_nsec;
	return elapsed_time > interval_nanosec;
}

long turn_time_decrease(size_t count)
{
	static long decrease = INIT_DECRESE_TIME;
	long sum_turn_time_decrease = decrease * count;
	for (size_t i = 0; i < count; i++)
	{
		sum_turn_time_decrease -= decrease--;
	}
	return sum_turn_time_decrease;
}
