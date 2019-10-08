#include <stdio.h>

#include "test.h"
#include "plotter.h"

int main()
{
    printf("========      Linear Regression      ========\n\n");
    printf("This application requires a maximised console window.\n\n");

    test();
    init_plotter(45, 200);

    printf("\n\n======== Completed Linear Regression ========\n\n");
    getchar();
    return 0;
}