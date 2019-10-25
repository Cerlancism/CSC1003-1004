#include <math.h>

#define SCALE_MAX 8.0f
#define SCALE_MID 0.1f
#define SCALE_SMALL 0.05f

#define STEP_MID 0.1f
#define STEP_SMALL 0.01f

static int fuzzyEquals(const float a, const float b, const float epsilon)
{
    return fabsf(a - b) <= epsilon;
}

void navigate(const char *const controller, float *xAxis, float *yAxis, float *scale)
{
    switch (*controller)
    {
    case '<':
        *xAxis -= 1;
        break;
    case '>':
        *xAxis += 1;
        break;
    case '^':
        *yAxis += 1;
        break;
    case 'v':
    case 'V':
        *yAxis -= 1;
        break;
    case '+':
        if (*scale > 1)
        {
            *scale -= 1;
        }
        else if (*scale > (SCALE_MID + 0.1))
        {
            *scale -= STEP_MID;

            if (fuzzyEquals(*scale, SCALE_MID, 0.001))
            {
                *scale = SCALE_MID;
            }
        }
        else if (*scale > SCALE_SMALL)
        {
            *scale -= STEP_SMALL;

            if (fuzzyEquals(*scale, SCALE_SMALL, 0.001f))
            {
                *scale = SCALE_SMALL;
            }
        }

        break;
    case '-':
        if (*scale >= 1 && *scale < SCALE_MAX)
        {
            *scale += 1;
        }
        else if (*scale < SCALE_MAX && *scale >= SCALE_MID)
        {
            *scale += STEP_MID;

            if (fuzzyEquals(*scale, 1, 0.001f))
            {
                *scale = 1;
            }
        }
        else if (*scale < SCALE_MAX && *scale >= (SCALE_SMALL - 0.01f))
        {
            *scale += STEP_SMALL;

            if (fuzzyEquals(*scale, SCALE_MID, 0.001f))
            {
                *scale = SCALE_MID;
            }
        }
        break;
    }
}