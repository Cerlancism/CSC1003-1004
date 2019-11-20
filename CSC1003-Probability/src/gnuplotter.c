/*

gnuplotter.c
Adapter for Gnuplot command line interface 
http://www.gnuplot.info/
http://www.gnuplotting.org/

*/

#include <stdio.h>
#include <stdlib.h>
#include "mathsUtils.h"

#define GNU_PLOTH_PATH "gnuplot"

/* Check the user's machine has Gnuplot intalled and runnable as a global process. */
int gnuplotter_exists()
{
    FILE *pipe = popen(GNU_PLOTH_PATH, "w");

    if (pipe != NULL)
    {
        if (pclose(pipe) == 0) /* Test if Gnuplot has opened and then closed successfully.*/
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
/* Launch the Gnuplot application. */
FILE *gnuplotter_pipe(const char *datafile, const char *histogramFile, const char *binFile, float m, float c, float gaussianBase, float mean, float sd, float multiplier, int partitions)
{
    float min = mean - 4 * sd;
    float max = mean + 4 * sd;
    float width = (max - min) / partitions;

    FILE *pipe = popen(GNU_PLOTH_PATH " -persistent", "w");

    if (pipe != NULL)
    {
        fprintf(pipe, "set fit quiet\n");
        fprintf(pipe, "set title 'Histogram overlaid with PDF, normalisation magnitude = %g'\n", multiplier);
        fprintf(pipe, "gauss(x)=a/(sqrt(2*pi)*sigma)*exp(-(x-mean)**2/(2*sigma**2)) \n");
        fprintf(pipe, "fit gauss(x) '%s' u 1:2 via a, sigma, mean\n", binFile);

        fprintf(pipe, "set style line 1 linecolor rgb '#0060ad' linetype 1 linewidth 1\n");
        fprintf(pipe, "set style line 2 linecolor rgb '#60ad00' linetype 1 linewidth 1\n");
        fprintf(pipe, "set style line 3 linecolor rgb '#ad0000' linetype 1 linewidth 1\n");
        fprintf(pipe, "set multiplot layout 2,1 rowsfirst\n");
        fprintf(pipe, "set xrange [-5:25]\n");
        fprintf(pipe, "set datafile separator ','\n");
        fprintf(pipe, "plot '%s' with points linestyle 2, %f * x + %f title 'y = %fx + %f' with lines linestyle 1\n", datafile, m, c, m, c);
        fprintf(pipe, "set xrange [%f:%f]\n", min, max);

        fprintf(pipe, "n = %d\n", partitions);
        fprintf(pipe, "width = %f\n", width);
        fprintf(pipe, "hist(x, width) = width * floor(x / width) + width / 2.0\n");
        fprintf(pipe, "set boxwidth width\n");
        fprintf(pipe, "set style fill solid 0.5 #fillstyle\n");
        fprintf(pipe,
                "plot '%s' u (hist($1,width)):(1.0) smooth freq w boxes lc rgb '#60ad00' title 'Histogram of noise n, partitions = %d', "
                "gauss(x) title sprintf(\"Best fitted line with μ = %%f, σ = %%f\", mean, sigma), "
                "%f * (exp(-0.5 * (((x - %f) / %f) ** 2))) title 'Gaussian function with μ = %f, σ = %f' with lines linestyle 3\n",
                histogramFile,
                partitions, gaussianBase * multiplier, mean, sd, mean, sd);
        fprintf(pipe, "unset multiplot\n");
    }
    else
    {
        printf("Failed to open Gnuplot!\n");
    }

    return pipe;
}
