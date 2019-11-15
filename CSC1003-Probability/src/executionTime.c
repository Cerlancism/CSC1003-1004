/*

executionTime.c
Utility to keep track time intervals

*/

#include <stdio.h>
#include <time.h>

#include "executionTime.h"

/* 
#define USE_SYSTEMTIME 
*/

static double getClock()
{
#ifdef USE_SYSTEMTIME
    return ((double)clock() / CLOCKS_PER_SEC) * 1000;
#else
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (t.tv_sec * 1e9 + t.tv_nsec) * 1e-6;
#endif
}

double timer_start(Interval *interval)
{
    interval->start = getClock();
}

double timer_end(Interval *interval)
{
    interval->end = getClock();
}

double timer_getInterval(const Interval *const interval)
{
    return interval->end - interval->start;
}

void timer_report(const Interval *const interval, const char *const description)
{
    printf("Execution Time for %s: %.2lf ms \n", description, timer_getInterval(interval));
}
