/*

mathsUtils.h provides some maths helper functions and fallback implementations which are not strictly available in c89

*/

/* Classic include guard */
#ifndef MATHSUTILS_h
#define MATHSUTILS_h

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
extern float roundf(float input);
#endif

/* Check if a float is close to a value based on the epsilon precision */
extern int fuzzyEqualsf(const float a, const float b, const float epsilon);

#endif
