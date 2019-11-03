/*
probability.c

The main entry point and regression, pdf computation point of the program.
*/

#include <stdio.h>  /* IO operations console/file/string */
#include <math.h>   /* pow round ceil floor */
#include <stdlib.h> /* malloc free */
#include <unistd.h> /* Parse cli options */

#include "plotter.h"    /* For plotting the graph on console */
#include "gnuplotter.h" /* For plotting the graph on gnu plot */
#include "navigator.h"  /* For nagivating the plotted graph */

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
} Coord2D;

Coord2D *coordinates;

typedef struct configuration
{
    char fileName[128];
    unsigned int lineCount;
    unsigned int consoleHeight;
    unsigned int consoleWidth;
} Configuration;

Configuration config;
/*
 \fn getRegressLine
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
void getRegressLine(const char *file, float *m, float *c, float *r, float *rr, float *standErrOfEstimate, float *minY, float *maxY)
{
    /* Declare and intialize line buffer, index as iterator for all lines in  */
    /* in stream file, summation of X, Y, X Square, Y Square, XY, YPrime and  */
    /* y - y' */
    char line_buf[LINE_BUFFER_SIZE];
    size_t index = 0;
    float sumX = 0.0f, sumY = 0.0f, sumXX = 0.0f;
    float sumYY = 0.0f, sumXY = 0.0f;
    float yPrime = 0.0f, yyPrimeDiffSum = 0.0f;
    FILE *fileStream = fopen(file, "r"); /* Open file with read permission*/

    if (fileStream == NULL) /* If fail to open file, exit program. */
    {
        printf("Error opening file: %s\n", file);
        exit(0);
    }
    /* At this point, file opened successfully, allocate SIZE no. coord2d objs*/
    coordinates = (Coord2D *)malloc(sizeof(Coord2D) * SIZE);
    if (coordinates == NULL) /* If allocation memory fail, exit program. */
    {
        printf("Exiting program. Failure in allocating memory in getRegressLine Function.\n");
        exit(1);
    }
    /* Read each line of the file and assign the float values to x and y */
    for (index = 0; (fgets(line_buf, LINE_BUFFER_SIZE, fileStream) != NULL) && (index < SIZE); index++)
    {
        sscanf(line_buf, "%f,%f", &coordinates[index].x, &coordinates[index].y);
    }

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
        yyPrimeDiffSum += (coordinates[index].y - yPrime) * (coordinates[index].y - yPrime);
    }
    /* Caclulate standard error of estimate and assign to pointee */
    *standErrOfEstimate = sqrt(yyPrimeDiffSum / (SIZE - 2));
    fclose(fileStream); /* Close file as best practice */
}

void displayPlot(float m, float c, float viewX, float viewY, float scale, float minY, float maxY)
{
    size_t i;
    float x, lineStep, labelPositionX, yTop, yMid, yBot;
    char equationLabel[30];
    float xStart = viewX * scale;
    float xLength = 25 * scale;
    float xEnd = xStart + xLength;
    float yStart = viewY * scale;
    float yLength = ceil(maxY - minY) * scale;
    float yToConsoleStep = yLength / PLOT_HEIGHT;

    plotter_init(PLOT_HEIGHT, PLOT_WIDTH, xStart, xLength, yStart, yLength);

    /* Plot the noise */
    for (i = 0; i < SIZE; i++)
    {
        plotter_printCoord("X", &coordinates[i].x, &coordinates[i].y);
    }

    /* Plot the line */
    lineStep = xLength / PLOT_WIDTH;
    for (x = xStart; x < xEnd; x += lineStep)
    {
        float y = m * x + c;
        plotter_printCoord("*", &x, &y);
    }

    /* Show a label in the graph */
    labelPositionX = xStart + xLength / 2;
    sprintf(equationLabel, "| y = %.2fx + %.2f |", m, c);
    yTop = m * labelPositionX + c - yToConsoleStep * 2, yMid = m * labelPositionX + c - yToConsoleStep * 1, yBot = m * labelPositionX + c - yToConsoleStep * 3;
    plotter_printCoord(equationLabel, &labelPositionX, &yTop); /* Print Equation label on middle of line graph. */
    plotter_printCoord(".------------------.", &labelPositionX, &yMid);
    plotter_printCoord("'------------------'", &labelPositionX, &yBot);

    /* Print the graph to the console */
    plotter_render();
    /* Release plotter buffer memory */
    plotter_dispose();
}

/* Initialise program configurations to default values */
void initConfig()
{
    sprintf(config.fileName, "%s", "Group1_8.txt");
    config.lineCount = 10000;
    config.consoleHeight = 20;
    config.consoleWidth = 60;
}

/* Process command line arguments to configure the program configurations if there are any */
void parseCommandLine(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv, ":f:l:r:c:h")) != -1)
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
        case 'h': /* Show a simple help message showing these configurable options */
            printf("-f [filename]\n-l [line count]\n-r [console height]\n-c [console rows]\n");
            exit(0);
            break;
        case '?':
            printf("Unknown option -%c.\n", optopt);
            break;
        }
    }
    printf("File: %s", config.fileName);
    printf(", Lines: %d", config.lineCount);
    printf(", Console Plot Height: %d", config.consoleHeight);
    printf(", Console Plot Width: %d", config.consoleWidth);
    puts("\n-h to display command line options");
}

int main(int argc, char **argv)
{
    /* Declare all the float variables for gradient, constant, etc... */
    float m = 0.0f, c = 0.0f, r = 0.0f, rr = 0.0f, standErrOfEstimate = 0.0f;
    /* char grid[SCALE][SCALE] = {" "}; */
    float minY = 0.0f;
    float maxY = 0.0f;

    float scale, viewX, viewY;

    char controlChar = '\0';

    initConfig();
    parseCommandLine(argc, argv);

    /* Use function and calculate regression line and get respective values */
    getRegressLine(config.fileName, &m, &c, &r, &rr, &standErrOfEstimate, &minY, &maxY);
    /* Print out of all the respective important values */
    printf("y = %f x + %f \n", m, c);
    printf("Correlation coefficient: %f \n", r);
    printf("Coefficient of determination: %f %% \n", rr);
    printf("Standard error of estimate: %f \n", standErrOfEstimate);

    scale = 1;
    viewX = -2;
    viewY = floor(minY);

    if (hasGNUPlot())
    {
        printf("Looks like you have GNU Plot installed, do you want to launch it? Y/N\n(This program will still alternatively plot on console as ASCII art)\n");
        controlChar = getchar();
        displayPlot(m, c, viewX, viewY, scale, minY, maxY);
        if (controlChar == 'Y' || controlChar == 'y')
        {
            printf("Exit GNU Plot. Type < > ^ v + - to pan and zoom the console graph. Current scaling: %.2f\n", 1 / scale);
            gnu_plot(config.fileName, m, c);
        }
        else
        {
            printf("Type < > ^ v + - to pan and zoom the graph. Current scaling: %.2f\n", 1 / scale);
        }
    }
    else
    {
        printf("GNU Plot not intalled, this program will plot on console as ASCII art.\n");
        displayPlot(m, c, viewX, viewY, scale, minY, maxY);
        printf("Type < > ^ v + - to pan and zoom the graph. Current scaling: %.2f\n", 1 / scale);
    }

    while (1)
    {
        controlChar = getchar();

        if (navigate(&controlChar, &viewX, &viewY, &scale))
        {
            system(CLEARCLS); /* Clear console screen */
            displayPlot(m, c, viewX, viewY, scale, minY, maxY);
            printf("Type < > ^ v + - to pan and zoom the graph. Current scaling: %.2f\n", 1 / scale);
        }
    }
    return 0;
}
