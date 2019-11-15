/*

gnuplotter.c
Adapter for GNU Plot command line interface 
http://www.gnuplot.info/
http://www.gnuplotting.org/

*/

#include <stdio.h>
#include <stdlib.h>
#include "mathsUtils.h"

#define GNU_PLOTH_PATH "gnuplot"

/* Check the user's machine has GNU Plot intalled and runnable as a global process. */
int gnuplotter_exists()
{
    FILE *pipe = popen(GNU_PLOTH_PATH, "w");

    if (pipe != NULL)
    {
        if (pclose(pipe) == 0) /* Test if GNU plot has opened and then closed successfully.*/
        {                      /* As so far the popen will return a non null pointer even if the process is not found. */
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
FILE *gnuplotter_pipe(const char *datafile, float m, float c, float gaussianBase, float mean, float sd)
{
    FILE *pipe = popen(GNU_PLOTH_PATH " -persistent", "w");

    if (pipe != NULL)
    {
        fprintf(pipe, "set style line 1 linecolor rgb '#0060ad' linetype 1 linewidth 1\n");
        fprintf(pipe, "set style line 2 linecolor rgb '#60ad00' linetype 1 linewidth 1\n");
        fprintf(pipe, "set style line 3 linecolor rgb '#ad0000' linetype 1 linewidth 1\n");
        fprintf(pipe, "set multiplot layout 2,1 rowsfirst\n");
        fprintf(pipe, "set xrange [-5:25]\n");
        fprintf(pipe, "set yrange [-15:35]\n");
        fprintf(pipe, "set datafile separator ','\n");
        fprintf(pipe, "plot '%s' with points linestyle 2, %f * x + %f title 'line' with lines linestyle 1\n", datafile, m, c);
        fprintf(pipe, "set xrange [-15:15]\n");
        fprintf(pipe, "set yrange [0:0.08]\n");
        fprintf(pipe, "plot %f * (%f ** (-1.0 * 0.5 * (((x - %f) / %f) * ((x - %f) / %f)))) title 'PDF' with lines linestyle 3\n", gaussianBase, M_E, mean, sd, mean, sd);
        fprintf(pipe, "unset multiplot\n");
    }
    else
    {
        printf("Failed to open GNU Plot!\n");
    }

    return pipe;
}
