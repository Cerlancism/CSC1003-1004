#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define LINE_BUFFER_SIZE 30

int main(int argc, char **argv)
{
    for (int i = 0; i < argc; i++)
    {
        printf("Arg %d: %s\n", i, argv[i]);
    }

    char line_buf[LINE_BUFFER_SIZE];

    FILE *fileStream = fopen("test.txt", "r");

    float x[10000];
    float y[10000];

    int maxLength = 0;
    for (int index = 0; fgets(line_buf, LINE_BUFFER_SIZE, fileStream) != NULL; index++)
    {
        sscanf(line_buf, "%f,%f", &x[index], &y[index]);
        int len = strlen(line_buf);
        maxLength = len > maxLength ? len : maxLength;
        printf("max length: %d\n", maxLength);
    }

    char out_buffer[LINE_BUFFER_SIZE];
    for (size_t i = 0; i < 10000; i++)
    {
        sprintf(out_buffer, "x: %g, y: %g", x[i], y[i]);

        //printf("%-30s line length: %d\n", out_buffer, strlen(out_buffer));
    }

    return 0;
}