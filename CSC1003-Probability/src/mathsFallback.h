/*

mathsFallback.h provides some maths functions that is not strictly available in c89

*/

/* Classic include guard */
#ifndef MATHSFallback_h
#define MATHSFallback_h

#if defined(__STDC__)
#define PREDEF_STANDARD_C_1989
#if defined(__STDC_VERSION__)
#define PREDEF_STANDARD_C_1990
#if (__STDC_VERSION__ >= 199409L)
#define PREDEF_STANDARD_C_1994
#endif
#if (__STDC_VERSION__ >= 199901L)
#define PREDEF_STANDARD_C_1999
#endif
#if (__STDC_VERSION__ >= 201710L)
#define PREDEF_STANDARD_C_2018
#endif
#endif
#endif

#ifndef PREDEF_STANDARD_C_1990 /* roundf only exits after c99*/
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

#endif
