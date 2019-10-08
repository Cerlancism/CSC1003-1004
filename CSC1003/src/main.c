#include <stdio.h>

#include "test.h"
#include "plotter.h"

int main()
{
    printf("========      Linear Regression      ========\n\n");
    printf("This application requires a maximised console window.\n\n");

    init_plotter(50, 200, 0, 20, -20, 100);
    render();
    test();

    printf("Please maximise console window and run again if the this looks weird.\n\n");
    printf("\n\n======== Completed Linear Regression ========\n\n");
    getchar();
    return 0;
}