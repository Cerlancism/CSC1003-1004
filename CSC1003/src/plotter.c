#include "test.h"
#include <stdio.h>

void init_plotter(int lines, int length)
{
    printf("Init Plotter\n\n\n");

    printf(" ^\n");
    for (size_t i = 0; i < lines; i++)
    {
        printf(" |");
        printf("\n");
    }
    printf(" +");

    for (size_t i = 0; i < length; i++)
    {
        printf("-");
    }
    printf(">\n\n");
    test();
}