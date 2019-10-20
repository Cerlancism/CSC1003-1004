#include "stdio.h"
#include "math.h"
#include "stdlib.h"

#include "plotter.h"

#include "getline.h" // mingw needs an alternative implementation of getline

#define SIZE 10000
#define SCALE 100

#define PLOT_HEIGHT 50
#define PLOT_WIDTH 200

#ifdef _WIN32
#define CLEARCLS "cls"
#else
#define CLEARCLS "clear"
#endif

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

    float scale = 1;
    float viewX = -2;
    float viewY = floor(minY);

    char controlChar = 0;

    while (1)
    {
        printf("This application requires a maximised console window.\n");
        float xStart = viewX * scale;
        float xLength = 25 * scale;
        float xEnd = xStart + xLength;
        float yStart = viewY * scale;
        float yLength = ceil(maxY - minY) * scale;
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
        plotter_dispose();

        printf("Please maximise console window and run again if the this looks weird.\n");
        printf("Type < > ^ v + - to pan and zoom the graph. current scale: %.2f\n", 1 / scale);

        controlChar = getchar();

        switch (controlChar) // Navigate the plot
        {
        case '<':
            viewX -= 1;
            break;
        case '>':
            viewX += 1;
            break;
        case '^':
            viewY += 1;
            break;
        case 'v':
        case 'V':
            viewY -= 1;
            break;
        case '+':
            if (scale > 1)
            {
                scale -= 1;
            }
            else if (scale > 0.1)
            {
                scale -= 0.1;
            }
            break;
        case '-':
            if (scale >= 1 && scale < 8)
            {
                scale += 1;
            }
            else if (scale < 8)
            {
                scale += 0.1;
            }
            break;
        default:
            break;
        }

        system(CLEARCLS); // Clear console screen
    }

    return 0;
}
