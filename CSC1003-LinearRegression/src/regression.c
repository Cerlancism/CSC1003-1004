#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#include "plotter.h"    // For plotting the graph on console
#include "gnuplotter.h" // For plotting the graph on gnu plot
#include "navigator.h"  // For nagivating the plotted graph

// system command to clear console
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

typedef struct coord2D
{
    float x;
    float y;
} Coord2D;

Coord2D *coordinates;

typedef struct configuration
{
    char fileName[128];
    int lineCount;
    int consoleHeight;
    int consoleWidth;
} Configuration;

Configuration config;

void getRegressLine(const char *file, float *m, float *c, float *r, float *rr, float *standErrOfEstimate, float *minY, float *maxY)
{
    char line_buf[LINE_BUFFER_SIZE];
    size_t index = 0;
    float sumX = 0.0f, sumY = 0.0f, sumXX = 0.0f, sumYY = 0.0f, sumXY = 0.0f, yPrime = 0.0f, yyPrimeDiffSum = 0.0f;
    FILE *fileStream = fopen(file, "r");

    if (fileStream == NULL)
    {
        printf("Error openning file: %s\n", file);
        exit(0);
    }

    coordinates = (Coord2D *)malloc(sizeof(Coord2D) * SIZE);
    if (coordinates == NULL)
    {
        printf("Exiting program. Failure in allocating memory in getRegressLine Function.\n");
        exit(0);
    }

    for (index = 0; (fgets(line_buf, LINE_BUFFER_SIZE, fileStream) != NULL) && (index < SIZE); index++)
    {
        sscanf(line_buf, "%f,%f", &coordinates[index].x, &coordinates[index].y);
    }

    for (index = 0; index < SIZE; ++index)
    {
        //printf("Coordinate %lu : %f , %f \n", index, coordinates[index].x, coordinates[index].y);
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
    *standErrOfEstimate = sqrt(yyPrimeDiffSum / (SIZE));
    fclose(fileStream);
    //free(coordinates);
}

void displayPlot(float m, float c, float viewX, float viewY, float scale, float minY, float maxY)
{
    float xStart = viewX * scale;
    float xLength = 25 * scale;
    float xEnd = xStart + xLength;
    float yStart = viewY * scale;
    float yLength = ceilf(maxY - minY) * scale;
    float yEnd = yStart + yEnd;
    float yToConsoleStep = yLength / PLOT_HEIGHT;

    plotter_init(PLOT_HEIGHT, PLOT_WIDTH, xStart, xLength, yStart, yLength);

    // Plot the noise
    for (int i = 0; i < SIZE; i++)
    {
        plotter_printCoord("X", &coordinates[i].x, &coordinates[i].y);
    }

    // Plot the line
    float lineStep = xLength / PLOT_WIDTH;
    for (float x = xStart; x < xEnd; x += lineStep)
    {
        float y = m * x + c;
        plotter_printCoord("*", &x, &y);
    }

    // Show a lable in the graph
    char equationLable[20];
    float lablePositionX = xStart + xLength / 2;
    sprintf(equationLable, "| y = %.2fx + %.2f |", m, c);
    float yTop = m * lablePositionX + c - yToConsoleStep * 2, yMid = m * lablePositionX + c - yToConsoleStep * 1, yBot = m * lablePositionX + c - yToConsoleStep * 3;
    plotter_printCoord(equationLable, &lablePositionX, &yTop); // Print Equation lable on middle of line graph.
    plotter_printCoord(".------------------.", &lablePositionX, &yMid);
    plotter_printCoord("'------------------'", &lablePositionX, &yBot);

    // Print the graph to the console
    plotter_render();
    // Release plotter buffer memory
    plotter_dispose();
}

// Initialise program configurations to default values
void initConfig()
{
    sprintf(config.fileName, "%s", "Group1_8.txt");
    config.lineCount = 10000;
    config.consoleHeight = 20;
    config.consoleWidth = 100;
}

// Process command line arguments to configure the program configurations if there are any
void parseCommandLine(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv, ":f:l:r:c:h")) != -1)
    {
        switch (opt)
        {
        case 'f':
            sprintf(config.fileName, "%s", optarg);
            break;
        case 'l':
            sscanf(optarg, "%d", &config.lineCount);
            break;
        case 'r':
            sscanf(optarg, "%d", &config.consoleHeight);
            break;
        case 'c':
            sscanf(optarg, "%d", &config.consoleWidth);
            break;
        case 'h':
            printf("-f [filename]\n-l [line count]\n-r [console height]\n-c [console rows]\n");
            exit(0);
            break;
        case '?':
            printf("Unkown option -%c.\n", optopt);
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
    initConfig();
    parseCommandLine(argc, argv);

    float m = 0.0f, c = 0.0f, r = 0.0f, rr = 0.0f, standErrOfEstimate = 0.0f; /* Regression line gradient and constant */
    /* char grid[SCALE][SCALE] = {" "}; */
    float maxX = 0.001f;
    float minY = 0.0f;
    float maxY = 0.0f;
    getRegressLine(config.fileName, &m, &c, &r, &rr, &standErrOfEstimate, &minY, &maxY);
    printf("Min Y: %f , Max Y: %f \n", minY, maxY);
    printf("y = %f x + %f \n", m, c);
    printf("Correlation coefficient: %f \n", r);
    printf("Coefficient of determination: %f %% \n", rr);
    printf("Standard error of estimate: %f \n", standErrOfEstimate);
    putchar(' ');
    for (int i = 0; i < SCALE; ++i)
    {
        putchar('-');
    }

    printf(" %.3f \n", maxX);
    printf(" x: %f , y: %f\n", (-0.0001f - c) / m, -0.0001f);

    float scale = 1;
    float viewX = -2;
    float viewY = floorf(minY);

    char controlChar = 0;

    printf("Do you have GNU Plot intalled? Y/N (The plot will always printed on console as ASCII art) ");
    controlChar = getchar();

    displayPlot(m, c, viewX, viewY, scale, minY, maxY);
    if (controlChar == 'Y' || controlChar == 'y')
    {
        printf("Exit GNU Plot, Type < > ^ v + - to pan and zoom the console graph. Current scaling: %.2f\n", 1 / scale);
        gnu_plot(config.fileName, m, c);
    }
    else
    {
        printf("Type < > ^ v + - to pan and zoom the graph. Current scaling: %.2f\n", 1 / scale);
    }
    getchar();

    while (1)
    {
        if (controlChar == '\n')
        {
            controlChar = getchar();
            continue;
        }
        controlChar = getchar();
        navigate(&controlChar, &viewX, &viewY, &scale);
        system(CLEARCLS); // Clear console screen
        displayPlot(m, c, viewX, viewY, scale, minY, maxY);
        printf("Type < > ^ v + - to pan and zoom the graph. Current scaling: %.2f\n", 1 / scale);

        //navigate(&controlChar, &viewX, &viewY, &scale);
    }

    return 0;
}
