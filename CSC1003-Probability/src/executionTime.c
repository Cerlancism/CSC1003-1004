/*

executionTime.c
Utility to keep track time intervals

*/

#include <stdio.h>
#include <time.h>

#include "executionTime.h"

/* Default using the highest precision timer clock_gettime CLOCK_MONOTONIC */

/* Enable this to use the less precise timeofday if the more precise clock_gettime(CLOCK_MONOTONIC, &_t) does not work as it needs runtime library, OS X 10.12 above or winpthread library.  
#define USE_TIMEVAL
*/

/* Enable this to use the less precise clock() as last fallback.
#define USE_CLOCKT 
*/

#ifdef USE_TIMEVAL
#include <sys/time.h>
#endif

#ifdef USE_CLOCKT
#warning "Using less precise clock() time for execution timer."
#endif

static double getClock(void)
{
#ifdef USE_CLOCKT
    return ((double)clock() / CLOCKS_PER_SEC) * 1000;
#elif defined(USE_TIMEVAL)
    static struct timeval _tv;
    gettimeofday(&_tv, NULL);
    return _tv.tv_sec * 1000.0 + _tv.tv_usec / 1000.0;
#else
    static struct timespec _ts;
    clock_gettime(CLOCK_MONOTONIC, &_ts);
    return _ts.tv_sec * 1000.0 + _ts.tv_nsec / 1000000.0;
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
    double result = timer_getInterval(interval);
    if (result == 0) /* If time interval is too short for the precision of current timer */
    {
        printf("Execution Time for %s: probably a lot less than 1 ms \n", description);
    }
    else
    {
        printf("Execution Time for %s: %.3f ms \n", description, result);
    }
}
