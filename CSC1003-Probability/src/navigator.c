#include "mathsUtils.h" /* fuzzyEquals */

#define SCALE_MAX 8.0f
#define SCALE_MID 0.1f
#define SCALE_SMALL 0.05f

#define STEP_MID 0.1f
#define STEP_SMALL 0.01f

#define EPSILION 0.0001f

/* Returns 1 if something has changed, else 0 */
int navigate(const char *const controller, float *xAxis, float *yAxis, float *scale, unsigned int *width, unsigned int *height)
{
    switch (*controller)
    {
    case 'a':
    case 'A':
        *xAxis -= 1;
        break;
    case 'd':
    case 'D':
        *xAxis += 1;
        break;
    case 'w':
    case 'W':
        *yAxis += 1;
        break;
    case 's':
    case 'S':
        *yAxis -= 1;
        break;
    case '+':
        if (*scale > 1)
        {
            *scale -= 1;
        }
        else if (*scale > (SCALE_MID + 0.1f))
        {
            *scale -= STEP_MID;

            if (fuzzyEqualsf(*scale, SCALE_MID, EPSILION))
            {
                *scale = SCALE_MID;
            }
        }
        else if (*scale > SCALE_SMALL)
        {
            *scale -= STEP_SMALL;

            if (fuzzyEqualsf(*scale, SCALE_SMALL, EPSILION))
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

            if (fuzzyEqualsf(*scale, 1, EPSILION))
            {
                *scale = 1;
            }
        }
        else if (*scale < SCALE_MAX && *scale >= (SCALE_SMALL - 0.01f))
        {
            *scale += STEP_SMALL;

            if (fuzzyEqualsf(*scale, SCALE_MID, EPSILION))
            {
                *scale = SCALE_MID;
            }
        }
        break;

    case '>':
        *width += 1;
        break;
    case '<':
        *width = clampi(10, 1000, *width - 1);
        break;
    case 'v':
        *height += 1;
        break;
    case '^':
        *height = clampi(10, 1000, *height - 1);
        break;
    default:
        return 0;
    }
    return 1;
}
