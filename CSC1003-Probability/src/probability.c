/*
probability.c

The main entry point and regression, pdf computation point of the program.
*/

#include <stdio.h> /* IO operations console/file/string */
#include <math.h>  /* pow round ceil floor */
#include <stdlib.h>
#include <unistd.h> /* Parse cli options */

#include "mathsUtils.h"
#include "consoleplotter.h" /* For plotting the graph on console */
#include "gnuplotter.h"     /* For plotting the graph on Gnuplot */
#include "navigator.h"      /* For nagivating the plotted graph */
#include "executionTime.h"  /* For execution time tracking */

/* system command to clear console */
#ifdef _WIN32
#define CLEARCLS "cls"
#else
#define CLEARCLS "clear"
#endif

#define SIZE config.lineCount
#define SCALE 100
#define PLOT_HEIGHT config.consoleHeight
#define PLOT_WIDTH config.consoleWidth
#define LINE_BUFFER_SIZE 30

#define HISTOGRAM_FILE "histogram.dat"
#define BIN_FILE "bins.dat"

/*
  \struct coord2D
  \brief A struct object for storing x and y coordinates
  \var float x-coordinate value
  \var float y-coordinate value
 */
typedef struct coord2D
{
    float x;
    float y;
    float noise;
} Coord2D;

typedef struct histogram
{
    float minNoise;
    float maxNoise;
    float interval;
    float meanNoise;
    float sdNoise;
    size_t size;
    int *bins;
} Histogram;

typedef struct dataset
{
    Coord2D *coordinates;
    size_t coordLength;
    Histogram *histogram;
} DataSet;

typedef struct configuration
{
    char fileName[128];
    unsigned int lineCount;
    unsigned int consoleHeight;
    unsigned int consoleWidth;
    float histogramPartitionSize;
} Configuration;

Configuration config;

/* Execution time trackers */
Interval dataFileReadTime;
Interval dataAnalysisTime;
Interval consoleplotTime;
Interval gnuplotTime;

/*
 \fn analyseDataset
 \brief Function for reading input file and calculating regression line
 \param file Pointer to the file stream
 \param m Pointer to the regression line's gradient float value
 \param c Pointer to the regression line's constant float value
 \param r Pointer to the correlation coefficient float value
 \param rr Pointer to the coefficient of determination float value
 \param standErrOfEstimate Pointer to standard error of estimate float value
 \param minY Pointer to minimum Y float value
 \param maxY Pointer to maximum Y float value
*/
DataSet analyseDataset(const char *file, float *m, float *c, float *r, float *rr, float *standErrOfEstimate, float *minY, float *maxY)
{
    /* Declare and intialize line buffer, index as iterator for all lines in  */
    /* in stream file, summation of X, Y, X Square, Y Square, XY, YPrime and  */
    /* y - y' */
    char line_buf[LINE_BUFFER_SIZE];
    size_t index = 0;
    float sumX = 0.0f, sumY = 0.0f, sumXX = 0.0f;
    float sumYY = 0.0f, sumXY = 0.0f;
    float yPrime = 0.0f, yyPrimeDiffSum = 0.0f;
    float sumXXMeanDiff = 0.0f;
    float sumNoise = 0.0f;
    float sumNoiseMeanNoiseDiff = 0.0f;
    int bias = 0;
    FILE *fileStream;
    Histogram *hist = (Histogram *)malloc(sizeof(Histogram));
    Coord2D *coordinates = (Coord2D *)malloc(sizeof(Coord2D) * SIZE);
    DataSet dataset;

    /* Allocate SIZE no. coord2d objs*/
    if (coordinates == NULL || hist == NULL) /* If allocation memory fail, exit program. */
    {
        printf("MEMORY FAILURE: Failure in allocating memory in analyseData Function.\n");
        exit(1);
    }

    hist->interval = config.histogramPartitionSize;

    timer_start(&dataFileReadTime);

    fileStream = fopen(file, "r"); /* Open file with read permission*/

    if (fileStream == NULL) /* If fail to open file, exit program. */
    {
        printf("FILE ERROR: Error opening file: %s\n", file);
        exit(1);
    }

    /* Read each line of the file and assign the float values to x and y */
    for (index = 0; (fgets(line_buf, LINE_BUFFER_SIZE, fileStream) != NULL) && (index < SIZE); index++)
    {
        sscanf(line_buf, "%f,%f", &coordinates[index].x, &coordinates[index].y);
    }

    fclose(fileStream); /* Close file as best practice */

    timer_end(&dataFileReadTime);

    timer_start(&dataAnalysisTime);

    /* Calculating maxY, min y and all the summation values */
    for (index = 0; index < SIZE; ++index)
    {
        if (coordinates[index].y >= *maxY)
        {
            *maxY = coordinates[index].y;
        }
        if (coordinates[index].y <= *minY)
        {
            *minY = coordinates[index].y;
        }
        sumX += coordinates[index].x;
        sumY += coordinates[index].y;
        sumXX += coordinates[index].x * coordinates[index].x;
        sumYY += coordinates[index].y * coordinates[index].y;
        sumXY += coordinates[index].x * coordinates[index].y;
    }
    /* Calculate all the values based on formulas and assign to the pointees */
    *m = ((SIZE * sumXY) - (sumX * sumY)) / ((SIZE * sumXX) - (sumX * sumX));
    *c = (sumY - (*m * sumX)) / SIZE;
    *r = ((SIZE * sumXY) - (sumX * sumY)) / sqrt(((SIZE * sumXX) - (sumX * sumX)) * ((SIZE * sumYY) - (sumY * sumY)));
    /* percentage of corelation coefficient squared  */
    *rr = ((*r) * (*r)) * 100.0f;
    /* Calculate Summation of (y - yprime)^2 */

    for (index = 0; index < SIZE; ++index)
    {
        yPrime = (*m) * (coordinates[index].x) + *c;
        coordinates[index].noise = (coordinates[index].y - yPrime);
        sumNoise += coordinates[index].noise;
        yyPrimeDiffSum += coordinates[index].noise * coordinates[index].noise;
        if (coordinates[index].noise >= hist->maxNoise)
            hist->maxNoise = coordinates[index].noise;
        if (coordinates[index].noise <= hist->minNoise)
            hist->minNoise = coordinates[index].noise;
    }
    /* Caclulate standard error of estimate and assign to pointee */
    *standErrOfEstimate = sqrt(yyPrimeDiffSum / (SIZE - 2));
    sumXXMeanDiff /= SIZE;
    hist->meanNoise = sumNoise / SIZE;
    /* Calculating histogram stuffz,by here min, max and interval r calculated*/
    hist->size = (int)(ceil((hist->maxNoise - hist->minNoise) / hist->interval));
    hist->bins = (int *)calloc(hist->size, sizeof(int));
    bias = (0.0f - hist->minNoise) / hist->interval;
    printf("bias: %i \n", bias);
    if (!(hist->bins))
        printf("MEMORY FAILURE: Error allocating bins for histogram. \n");
    for (index = 0; index < SIZE; ++index)
    {
        ++(hist->bins)[(int)(coordinates[index].noise / hist->interval + bias)];
        sumNoiseMeanNoiseDiff += (coordinates[index].noise - hist->meanNoise) * (coordinates[index].noise - hist->meanNoise);
    }
    sumNoiseMeanNoiseDiff /= (SIZE - 1);
    hist->sdNoise = sqrt(sumNoiseMeanNoiseDiff);

    dataset.coordinates = coordinates;
    dataset.coordLength = SIZE;
    dataset.histogram = hist;

    timer_end(&dataAnalysisTime);
    return dataset;
}

void showConsolePlot(float m, float c, float linearXStart, float linearYStart, float scale, float minY, float maxY, float displayOffsetX, float displayOffsetY, DataSet dataset)
{
    Coord2D *coordinates = dataset.coordinates;
    Histogram *hist = dataset.histogram;

    size_t i, lableLength;
    float x, y, lineStep;
    char equationLabel[30], equationLabelBorder[30];
    float xStart = (linearXStart + displayOffsetX) * scale;
    float xLength = 30 * scale;
    float xEnd = xStart + xLength;
    float yStart = (linearYStart + displayOffsetY) * scale;
    float yLength = ceil(maxY - minY) * scale;
    float antiAliasLevel = 8;

    float guassianMultiplier = SIZE * hist->interval;
    float pdfStart = roundf(hist->meanNoise - hist->sdNoise * 4);
    float pdfWidth = roundf(hist->sdNoise * 8);
    float histCurveHeight = gaussianHeight(hist->sdNoise);
    float pdfHeight = histCurveHeight * (guassianMultiplier + guassianMultiplier * 0.1);
    float pdfYStep = pdfHeight / PLOT_WIDTH;
    float pdfXStep = pdfWidth / PLOT_WIDTH;
    char *barA = "X", *barB = "x"; /* Histogram bar patterns */
    int barAlternator = 0;

    consoleplotter_init(PLOT_HEIGHT, PLOT_WIDTH, xStart, xLength, yStart, yLength);

    /* Plot the noise */
    for (i = 0; i < SIZE; i++)
    {
        consoleplotter_printCoord("x", &coordinates[i].x, &coordinates[i].y);
    }

    /* Plot the line */
    lineStep = xLength / PLOT_WIDTH;
    for (x = xStart; x < xEnd; x += lineStep / antiAliasLevel)
    {
        y = m * x + c;
        consoleplotter_printCoord("+", &x, &y);
    }

    /* Show a label in the graph */
    sprintf(equationLabel, "| + : y = %.2fx + %.2f |", m, c);
    consoleplotter_printText(equationLabel, 12, 3); /* Print Equation label on top left of line graph */
    lableLength = strlen(equationLabel);
    consoleplotter_printText("|", 12, 2);
    consoleplotter_printText("x : noise", 12 + 2, 2);
    consoleplotter_printText("|", 12 + lableLength - 1, 2);
    memset(equationLabelBorder, '.', sizeof(char) * lableLength);
    equationLabelBorder[lableLength] = '\0';
    consoleplotter_printText(equationLabelBorder, 12, 1);
    memset(equationLabelBorder, '\'', sizeof(char) * lableLength);
    consoleplotter_printText(equationLabelBorder, 12, 4);

    /* Print the graph to the console */
    consoleplotter_render();
    /* Reset buffer memory for gaussian plotting */
    consoleplotter_clear();

    consoleplotter_init(PLOT_HEIGHT, PLOT_WIDTH, (pdfStart + displayOffsetX) * scale, pdfWidth * scale, (0 + displayOffsetY * pdfYStep) * scale, pdfHeight * scale);

    /* Plot the histogram */
    for (i = 0; i < hist->size; i++)
    {
        float yy, xx;                                 /* Sub coordinates for filling the histogram bar */
        char *bar = barAlternator == 0 ? barA : barB; /* Alternating bar graph pattern */
        x = hist->minNoise + hist->interval * i;
        y = hist->bins[i];

        for (yy = y; yy > 0; yy -= pdfYStep)
        {
            float barOffset = x + hist->interval / 4;
            float barEnd = barOffset + hist->interval;
            for (xx = barOffset; xx < barEnd; xx += pdfXStep)
            {
                consoleplotter_printCoord(bar, &xx, &yy);
            }
        }
        barAlternator = barAlternator == 0 ? 1 : 0;
    }

    /* Plot the gaussian function */
    for (x = pdfStart; x < pdfStart + pdfWidth; x += pdfXStep / antiAliasLevel)
    {
        y = gaussianPower(&histCurveHeight, &hist->meanNoise, &hist->sdNoise, &x) * guassianMultiplier;
        consoleplotter_printCoord("+", &x, &y);
    }

    consoleplotter_render();

    /* Release plotter buffer memory */
    consoleplotter_dispose();
}

/* Process command line arguments to configure the program configurations if there are any */
void parseCommandLine(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv, ":f:l:r:c:i:h")) != -1)
    {
        switch (opt)
        {
        case 'f': /* Name of data file */
            sprintf(config.fileName, "%s", optarg);
            break;
        case 'l': /* Lines to scan for the data file */
            sscanf(optarg, "%u", &config.lineCount);
            break;
        case 'r': /* Number of console rows to allocate for the ASCII Art plotting */
            sscanf(optarg, "%u", &config.consoleHeight);
            break;
        case 'c': /* Number of console columns to allocate for the ASCII Art plotting */
            sscanf(optarg, "%u", &config.consoleWidth);
            break;
        case 'i': /* Partition size of histogram */
            sscanf(optarg, "%f", &config.histogramPartitionSize);
            break;
        case 'h': /* Show a simple help message showing these configurable options */
            printf("-f [filename]\n-l [line count]\n-r [console height]\n-c [console rows]\n");
            exit(0);
            break;
        case '?':
            printf("Unknown option -%c.\n", optopt);
            break;
        }
    }
}

void probability()
{
    /* Declare all the float variables for gradient, constant, etc... */
    float m = 0.0f, c = 0.0f, r = 0.0f, rr = 0.0f, standErrOfEstimate = 0.0f;
    float minY = 0.0f;
    float maxY = 0.0f;
    float scale = 1.0f, XOffset = 0.0f, YOffset = 0.0f;
    char controlChar = '\0';

    DataSet dataset;

    FILE *gnuplotpipe = NULL;
    FILE *histFile;
    FILE *binsFile;

    /* Use function and calculate regression line, histogram, pdf and get respective values */
    dataset = analyseDataset(config.fileName, &m, &c, &r, &rr, &standErrOfEstimate, &minY, &maxY);
    /* Print out of all the respective important values */
    printf("Linear line: f(x) = %fx + %f \n", m, c);
    printf("Correlation coefficient: %f \n", r);
    printf("Coefficient of determination: %f %% \n", rr);
    printf("Standard error of estimate: %f \n", standErrOfEstimate);
    printf("Histogram's Mean Noise: %f , Standard Deviation of Noise: %f \n", dataset.histogram->meanNoise, dataset.histogram->sdNoise);
    printf("Histogram's Min Noise: %f , Max Noise: %f , Interval: %f \n", dataset.histogram->minNoise, dataset.histogram->maxNoise, dataset.histogram->interval);

    if (gnuplotter_exists())
    {
        printf("Looks like you have Gnuplot installed, do you want to launch it? Y/N\n(This program will still alternatively plot on console as ASCII art)\n");
        controlChar = getchar();
    }
    else
    {
        printf("Gnuplot not intalled, this program will plot on console as ASCII art.\n");
    }

    timer_start(&consoleplotTime);
    showConsolePlot(m, c, -5.0f, floor(minY), scale, minY, maxY, XOffset, YOffset, dataset);
    timer_end(&consoleplotTime);

    timer_report(&dataFileReadTime, "File Reading");
    timer_report(&dataAnalysisTime, "Regression and PDF Calculation");
    timer_report(&consoleplotTime, "Console Plotting");

    printf("Total Execution Time with Console Plotting: %0.2f ms\n", timer_getInterval(&dataFileReadTime) + timer_getInterval(&dataAnalysisTime) + timer_getInterval(&consoleplotTime));

    /* User has consented to open Gnuplot. */
    if (controlChar == 'Y' || controlChar == 'y')
    {
        size_t i;
        timer_start(&gnuplotTime);
        histFile = fopen(HISTOGRAM_FILE, "w");
        binsFile = fopen(BIN_FILE, "w");

        if (histFile == NULL || binsFile == NULL)
        {
            puts("FILE ERROR: Error writing temp histogram and bins data file!");
            exit(1);
        }

        /* Store noise values to a data file for gnuplot to read */
        for (i = 0; i < SIZE; i++)
        {
            fprintf(histFile, "%f\n", dataset.coordinates[i].noise);
        }

        /* Store bin values to a data file for gnuplot to read */
        for (i = 0; i < dataset.histogram->size; i++)
        {
            fprintf(binsFile, "%g %d\n", dataset.histogram->minNoise + dataset.histogram->interval * i, dataset.histogram->bins[i]);
            fprintf(binsFile, "%g, %d\n", dataset.histogram->maxNoise - dataset.histogram->interval * i, dataset.histogram->bins[dataset.histogram->size - 1 - i]);
        }

        fclose(binsFile);
        fclose(histFile);
        gnuplotpipe = gnuplotter_pipe(config.fileName, HISTOGRAM_FILE, BIN_FILE, m, c, gaussianHeight(dataset.histogram->sdNoise), dataset.histogram->meanNoise, dataset.histogram->sdNoise, SIZE * dataset.histogram->interval, dataset.histogram->size);
        fflush(gnuplotpipe);
        timer_end(&gnuplotTime);
        timer_report(&gnuplotTime, "Gnuplot Plotting");
        printf("Total Execution Time with Gnuplot Plotting: %0.2f ms\n", timer_getInterval(&dataFileReadTime) + timer_getInterval(&dataAnalysisTime) + timer_getInterval(&gnuplotTime));
    }
    printf("Type W A S D + - to pan and zoom the graph, < > ^ v to resize the graph. Current scaling: %.2f. q to exit.\n", 1 / scale);

    while (controlChar != 'q')
    {
        controlChar = getchar();

        timer_start(&consoleplotTime);
        if (navigate(&controlChar, &XOffset, &YOffset, &scale, &config.consoleWidth, &config.consoleHeight))
        {
            system(CLEARCLS); /* Clear console screen */
            showConsolePlot(m, c, -5, floor(minY), scale, minY, maxY, XOffset, YOffset, dataset);
            timer_end(&consoleplotTime);
            timer_report(&consoleplotTime, "Replotting Console Plot");
            printf("Type W A S D + - to pan and zoom the graph, < > ^ v to resize the graph. Current scaling: %.2f. q to exit.\n", 1 / scale);
        }
    }

    puts("Terminating program...");

    if (gnuplotpipe)
    {
        printf("Close Gnuplot Window as it is still open.");
        pclose(gnuplotpipe);
        printf("\r%64s\r", " "); /* Erase the current line. */

        if (remove("fit.log"))
            printf("Unable to delete one gnuplot log file.");
    }

    free(dataset.coordinates);
    free(dataset.histogram);
    free(dataset.histogram->bins);

    puts("Program terminated successfully!");
}

int main(int argc, char **argv)
{
    /* Initialise default configuration values */
    sprintf(config.fileName, "%s", "Group1_8.txt");
    config.lineCount = 10000;
    config.consoleHeight = 20;
    config.consoleWidth = 60;
    config.histogramPartitionSize = 1.0f;

    parseCommandLine(argc, argv);

    /* Display the configuration */
    printf("File: %s", config.fileName);
    printf(", Lines: %d", config.lineCount);
    printf(", Console Plot Height: %d", config.consoleHeight);
    printf(", Console Plot Width: %d", config.consoleWidth);
    printf(", Histogram Partition Size (Interval): %f\n", config.histogramPartitionSize);

    /* Execute the task of this program */
    probability();

    return 0;
}
