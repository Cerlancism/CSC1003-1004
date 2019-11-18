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

#ifndef USE_SYSTEMTIME
static struct timespec _t;
#endif

static double getClock()
{
#ifdef USE_SYSTEMTIME
    return ((double)clock() / CLOCKS_PER_SEC) * 1000;
#else
    clock_gettime(CLOCK_MONOTONIC, &_t);
    return (_t.tv_sec * 1e9 + _t.tv_nsec) * 1e-6;
#endif
}

double timer_start(Interval *interval)
{
    return interval->start = getClock();
}

double timer_end(Interval *interval)
{
    return interval->end = getClock();
}

double timer_getInterval(const Interval *const interval)
{
    return interval->end - interval->start;
}

void timer_report(const Interval *const interval, const char *const description)
{
    printf("Execution Time for %s: %.2f ms \n", description, timer_getInterval(interval));
}
