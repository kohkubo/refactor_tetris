#include "tetris.h"

#include <time.h>
#include <sys/time.h>

extern long g_turn_time_nanosec;
extern struct timespec g_time_spec;

int64_t get_nsec(const struct timespec *ts)
{
	return ts->tv_sec * 1000000000 + ts->tv_nsec;
}

bool has_to_update()
{
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	int64_t now_nsec = get_nsec(&now);
	int64_t prev_nsec = get_nsec(&g_time_spec);
	int64_t elapsed_time = now_nsec - prev_nsec;
	return elapsed_time > g_turn_time_nanosec;
}

void update_turn_time(size_t count)
{
	static long decrease = INIT_DECRESE_TIME;
	for (size_t i = 0; i < count; i++)
	{
		g_turn_time_nanosec -= decrease--;
	}
}
