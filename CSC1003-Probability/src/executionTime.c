/*

executionTime.c
Utility to keep track time intervals

*/

#include <stdio.h>
#include <time.h>

#include "executionTime.h"

/* Enable this to use the less precise timeofday if the more precise clock_gettime(CLOCK_MONOTONIC, &_t) does not work as needs runtime or pthread library.  
#define USE_TIMEOFDAY
*/

/* Enable this to use the less precise cpu tick as last fallback.
#define USE_SYSTEMTIME 
*/

#ifdef USE_SYSTEMTIME
#warning "Using less precise cpu tick time for execution timer."
#endif

#ifdef USE_TIMEOFDAY
#include <sys/time.h>
#endif

static double getClock()
{
#ifdef USE_SYSTEMTIME
    return ((double)clock() / CLOCKS_PER_SEC) * 1000;
#elif defined(USE_TIMEOFDAY)
    static struct timeval _tv;
    gettimeofday(&_tv, NULL);
    return _tv.tv_sec * 1000 + _tv.tv_usec / 1000.0;
#else
    static struct timespec _ts;
    clock_gettime(CLOCK_MONOTONIC, &_ts);
    return _ts.tv_sec * 1000 + _ts.tv_nsec / 1000000.0;
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
    if (result == 0) /* Time interval is too short for the precision of current timer */
    {
        printf("Execution Time for %s: much less than 1 ms \n", description);
    }
    else
    {
        printf("Execution Time for %s: %.3f ms \n", description, result);
    }
}
