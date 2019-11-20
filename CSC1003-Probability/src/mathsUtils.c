
#include <math.h>
#include "mathsUtils.h"

#ifndef PREDEF_STANDARD_C_1990 /* roundf only exits after c99 */
float roundf(float input)
{
    if (input < 0.0)
    {
        return (float)((int)(input - 0.5));
    }
    else
    {
        return (float)((int)(input + 0.5));
    }
}
#endif

/* Check if a float is close to a value based on the epsilon precision */
int fuzzyEqualsf(const float a, const float b, const float epsilon)
{
    return fabsf(a - b) <= epsilon;
}

float clampf(const float lower, const float upper, const float value)
{
    return value >= lower && value <= upper ? value : value < lower ? lower : upper;
}

int clampi(const int lower, const int upper, const int value)
{
    return value >= lower && value <= upper ? value : value < lower ? lower : upper;
}

float gaussianHeight(float sd)
{
    return (1.0 / (sqrt(2.0 * M_PI) * sd));
}

float gaussianPower(const float *const height, const float *const mean, const float *const sd, const float *const x)
{
    return *height * exp(-0.5 * pow((*x - *mean) / *sd, 2.0));
}
