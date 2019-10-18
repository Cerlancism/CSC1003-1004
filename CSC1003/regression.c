#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#define SIZE 10000
#define SCALE 100
typedef struct coord2D
{
    float x;
    float y;
} coord2D;


void getRegressLine(const char * file,float * m, float * c, float * r, float * rr, float * standErrOfEstimate, float * minY, float * maxY)
{
    char * line_buf = NULL;
    size_t line_buf_size = 0, index = 0;
    float sumX = 0.0f, sumY = 0.0f, sumXX = 0.0f, sumYY = 0.0f, sumXY = 0.0f, yPrime = 0.0f, yyPrimeDiffSum = 0.0f;
    FILE * fileStream = fopen(file, "r");
    coord2D * coordinates = (coord2D *) malloc(sizeof(coord2D) * SIZE);
    if(!coordinates)
    {
        printf("Exiting program. Failure in allocating memory in getRegressLine Function.\n");
        exit(0);
    }
    for(;(getline(&line_buf, &line_buf_size, fileStream) >= 0) && (index < SIZE); ++index)
        sscanf(line_buf, "%f,%f", &coordinates[index].x, &coordinates[index].y);
    
    for(index = 0; index < SIZE; ++index)
    {
        printf("Coordinate %lu : %f , %f \n", index, coordinates[index].x, coordinates[index].y);
        if(coordinates[index].y >= *maxY)
            *maxY = coordinates[index].y;
        if(coordinates[index].y <= *minY)
            *minY = coordinates[index].y;
        sumX += coordinates[index].x;
        sumY += coordinates[index].y;
        sumXX += coordinates[index].x * coordinates[index].x;
        sumYY += coordinates[index].y * coordinates[index].y;
        sumXY += coordinates[index].x * coordinates[index].y;
    }
   
    *m = ((SIZE * sumXY)-(sumX * sumY))/((SIZE * sumXX)-(sumX * sumX));
    *c = (sumY-(*m * sumX))/SIZE;
    *r = ((SIZE * sumXY) - (sumX * sumY))/sqrt(((SIZE * sumXX) - (sumX * sumX))*((SIZE * sumYY) - (sumY * sumY)));
    /* percentage of corelation coefficient squared  */
    *rr = ((*r) * (*r)) * 100.0f;
    /* Calculate Summation of (y - yprime)^2 */
    for(index = 0; index < SIZE; ++index)
    {
        yPrime = (*m) * (coordinates[index].x) + *c;
        yyPrimeDiffSum += (coordinates[index].y - yPrime) * (coordinates[index].y - yPrime);
    }
    *standErrOfEstimate = sqrt(yyPrimeDiffSum/(SIZE));
    fclose(fileStream);
    free(coordinates);
}

int main(void)
{
    float m =0.0f, c = 0.0f, r = 0.0f, rr = 0.0f, standErrOfEstimate = 0.0f; /* Regression line gradient and constant */
    /* char grid[SCALE][SCALE] = {" "}; */
    int i = 0;
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
    for(;i<SCALE;++i)
        printf("-");
    printf(" %.3f \n", maxX);
    printf(" x: %f , y: %f", (-0.0001f - c)/m , -0.0001f);
    
    
    
    return 0;
}