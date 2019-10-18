#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "test.h"
#include "plotter.h"

int main()
{
    printf("========      Linear Regression      ========\n\n");
    printf("This application requires a maximised console window.\n\n");

    plotter_init(50, 100, -3, 15, -10, 41);

    srand(time(NULL));

    for (int i = 0; i < 50; i++)
    {
        switch (rand() % 3)
        {
        case 1:
            plotter_printCoord("XD", rand() / (float)RAND_MAX * 15 - 3, rand() / (float)RAND_MAX * 41 - 10);
            break;
        case 2:
            plotter_printCoord("lol", rand() / (float)RAND_MAX * 15 - 3, rand() / (float)RAND_MAX * 41 - 10);
            break;

        default:
            plotter_printCoord("LMAO", rand() / (float)RAND_MAX * 15 - 3, rand() / (float)RAND_MAX * 41 - 10);
            break;
        }
    }

    plotter_render();
    test();

    printf("Please maximise console window and run again if the this looks weird.\n\n");
    printf("\n\n======== Completed Linear Regression ========\n\n");
    //getchar();
    return 0;
}