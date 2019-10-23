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
        else if (*scale > 0.11)
        {
            *scale -= 0.1;

            if (*scale - 0.1 < 0.01)
            {
                *scale = 0.1;
            }
        }
        else if (*scale > 0.05)
        {
            *scale -= 0.01;

            if (*scale - 0.05 < 0.001)
            {
                *scale = 0.05;
            }
        }

        break;
    case '-':
        if (*scale >= 1 && *scale < 8)
        {
            *scale += 1;
        }
        else if (*scale < 8 && *scale >= 0.1)
        {
            *scale += 0.1;

            if (1 - *scale < 0.01)
            {
                *scale = 1;
            }
        }
        else if (*scale < 8 && *scale >= 0.04)
        {
            *scale += 0.01;

            if (0.1 - *scale < 0.001)
            {
                *scale = 0.1;
            }
        }
        break;
    }
}