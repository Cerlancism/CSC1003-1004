#include "stdio.h"
#include "math.h"
#include "stdlib.h"

#include "plotter.h"

#include "getline.h" // mingw needs an alternative implementation of getline

#define SIZE 10000
#define SCALE 100

#define PLOT_HEIGHT 50
#define PLOT_WIDTH 200

typedef struct coord2D
{
    float x;
    float y;
} coord2D;

coord2D *coordinates;

void getRegressLine(const char *file, float *m, float *c, float *r, float *rr, float *standErrOfEstimate, float *minY, float *maxY)
{
    char *line_buf = NULL;
    size_t line_buf_size = 0, index = 0;
    float sumX = 0.0f, sumY = 0.0f, sumXX = 0.0f, sumYY = 0.0f, sumXY = 0.0f, yPrime = 0.0f, yyPrimeDiffSum = 0.0f;
    FILE *fileStream = fopen(file, "r");
    coordinates = (coord2D *)malloc(sizeof(coord2D) * SIZE);
    if (!coordinates)
    {
        printf("Exiting program. Failure in allocating memory in getRegressLine Function.\n");
        exit(0);
    }
    for (; (getline(&line_buf, &line_buf_size, fileStream) >= 0) && (index < SIZE); ++index)
        sscanf(line_buf, "%f,%f", &coordinates[index].x, &coordinates[index].y);

    for (index = 0; index < SIZE; ++index)
    {
        //printf("Coordinate %lu : %f , %f \n", index, coordinates[index].x, coordinates[index].y);
        if (coordinates[index].y >= *maxY)
            *maxY = coordinates[index].y;
        if (coordinates[index].y <= *minY)
            *minY = coordinates[index].y;
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

int main(void)
{
    float m = 0.0f, c = 0.0f, r = 0.0f, rr = 0.0f, standErrOfEstimate = 0.0f; /* Regression line gradient and constant */
    /* char grid[SCALE][SCALE] = {" "}; */
    float maxX = 0.001f;
    float minY = 0.0f;
    float maxY = 0.0f;
    getRegressLine("Group1_8.txt", &m, &c, &r, &rr, &standErrOfEstimate, &minY, &maxY);
    printf("Min Y: %f , Max Y: %f \n", minY, maxY);
    printf("y = %f x + %f \n", m, c);
    printf("Correlation coefficient: %f \n", r);
    printf("Coefficient of determination: %f %% \n", rr);
    printf("Standard error of estimate: %f \n", standErrOfEstimate);
    printf(" ");
    for (int i; i < SCALE; ++i)
        printf("-");
    printf(" %.3f \n", maxX);
    printf(" x: %f , y: %f\n", (-0.0001f - c) / m, -0.0001f);

    printf("\n\n========      Linear Regression      ========\n\n");
    printf("This application requires a maximised console window.\n\n");

    float scale = 1;

    float xStart = 0 * scale;
    float xLength = 20 * scale;
    float xEnd = xStart + xLength;

    float yStart = -12 * scale;
    float yLength = 40 * scale;
    float yEnd = yStart + yEnd;
    float yToConsoleStep = yLength / PLOT_HEIGHT;

    plotter_init(PLOT_HEIGHT, PLOT_WIDTH, xStart, xLength, yStart, yLength);

    // Plot the noise
    for (int i = 0; i < SIZE; i++)
    {
        plotter_printCoord("X", coordinates[i].x, coordinates[i].y);
    }

    // Plot the line
    for (float x = xStart; x < xEnd; x += (xEnd / PLOT_WIDTH))
    {
        plotter_printCoord("*", x, m * x + c);
    }

    // Show a lable in the graph
    char equationLable[20];
    float lablePositionX = xStart + xLength / 2;
    sprintf(equationLable, "| y = %.2fx + %.2f |", m, c);
    plotter_printCoord(equationLable, lablePositionX, m * lablePositionX + c - yToConsoleStep * 2); // Print Equation lable on middle of line graph.
    plotter_printCoord(" ------------------ ", lablePositionX, m * lablePositionX + c - yToConsoleStep * 1);
    plotter_printCoord(" ------------------ ", lablePositionX, m * lablePositionX + c - yToConsoleStep * 3);

    // Print the graph to the console
    plotter_render();

    printf("Please maximise console window and run again if the this looks weird.\n\n");
    printf("\n\n======== Completed Linear Regression ========\n\n");

    return 0;
}
