typedef struct interval
{
    double start;
    double end;
} Interval;

extern double timer_start(Interval *interval);
extern double timer_end(Interval *interval);
extern void timer_report(const Interval *const interval, const char *const description);
