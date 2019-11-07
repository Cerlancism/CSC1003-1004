/*

executionTime.c
Utility to keep track time intervals

*/

#include <stdio.h>
#include <time.h>

#include "executionTime.h"

static double getClock()
{
    return (double)clock() / CLOCKS_PER_SEC;
}

double timer_start(Interval *interval)
{
    (*interval).start = getClock();
}

double timer_end(Interval *interval)
{
    (*interval).end = getClock();
}

void timer_report(const Interval *const interval, const char *const description)
{
    printf("Execution Time for %s: %.2lf ms\n", description, ((*interval).end - (*interval).start) * 1000);
}