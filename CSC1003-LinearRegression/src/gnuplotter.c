#include <stdio.h>
#include <stdlib.h>

#define GNU_PLOTH_PATH "gnuplot -persistent"

void gnu_plot(const char *datafile, float m, float c)
{
    FILE *pipe;

    if ((pipe = popen(GNU_PLOTH_PATH, "w")) != NULL)
    {
        fprintf(pipe, "set terminal wxt size 1280,720 persist\n");
        fprintf(pipe, "set xrange [-5:25]\n");
        fprintf(pipe, "set yrange [-15:35]\n");
        fprintf(pipe, "set style line 1 linecolor rgb '#0060ad' linetype 1 linewidth 1\n");
        fprintf(pipe, "set style line 2 linecolor rgb '#60ad00' linetype 1 linewidth 1\n");
        fprintf(pipe, "m = %f\n", m);
        fprintf(pipe, "c = %f\n", c);
        fprintf(pipe, "f(x) =  m * x + c\n");
        fprintf(pipe, "set datafile separator ','\n");
        fprintf(pipe, "plot '%s' with points linestyle 2,\\\n", datafile);
        fprintf(pipe, "f(x) title 'line' with lines linestyle 1\n");
    }
    else
    {
        printf("No GNU Plot Installed!\n");
    }

    if (pclose(pipe) != 0)
    {
        printf("No GNU Plot Installed!\n");
    }
    else
    {
        printf("GNU run was successful!\n");
    }
}