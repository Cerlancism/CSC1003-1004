/*

gnuplotter.c
Adapter for GNU Plot command line interface 
http://www.gnuplot.info/

*/

#include <stdio.h>
#include <stdlib.h>

#define GNU_PLOTH_PATH "gnuplot"

/* Check the user's machine has GNU Plot intalled and runnable as a global process. */
int hasGNUPlot()
{
    FILE *pipe = popen(GNU_PLOTH_PATH, "w");

    if (pipe != NULL)
    {
        if (pclose(pipe) == 0) /* Test if GNU plot has opened and then closed successfully. As so far the popen will return a non null pointer even if the process is not found. */
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

/* Launch the GNU Plot application. */
void gnu_plot(const char *datafile, float m, float c)
{
    FILE *pipe = popen(GNU_PLOTH_PATH " -persistent", "w");

    if (pipe != NULL)
    {
        fprintf(pipe, "set terminal qt size 1280,720\n");
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
        printf("Failed to open GNU Plot!\n");
    }

    if (pclose(pipe) != 0)
    {
        printf("Error in executing GNU Plot!\n");
    }
    else
    {
        printf("GNU run was successful!\n");
    }
}
