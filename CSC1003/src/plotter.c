#include <stdio.h>
#include <stdlib.h>

#include "test.h"

static int _rows;
static int _columns;

static char **_displayBuffer;

// Initialise the plotter display size.
void plotter_init(int rows, int colums, int xStart, unsigned int xLength, int yStart, unsigned int yLength)
{
    printf("Init Plotter\n\n\n");

    _rows = rows;
    _columns = colums;

    // Allocate memory for display buffer.
    _displayBuffer = malloc(sizeof(char *) * _rows);

    for (int i = 0; i < _rows; i++)
    {
        _displayBuffer[i] = malloc(sizeof(char) * _columns);
    }

    // Initialise an ASCII art box
    _displayBuffer[0][0] = ' ';
    for (size_t i = 1; i < _columns - 2; i++)
    {
        _displayBuffer[0][i] = '_';
    }
    _displayBuffer[0][_columns - 2] = '\0';

    for (size_t i = 1; i < (_rows - 1); i++)
    {
        _displayBuffer[i][0] = '|';
        for (size_t j = 1; j < _columns - 2; j++)
        {
            _displayBuffer[i][j] = ' ';
        }
        _displayBuffer[i][_columns - 2] = '|';
        _displayBuffer[i][_columns - 1] = '\0';
    }

    _displayBuffer[_rows - 1][0] = '+';
    for (size_t i = 1; i < _columns - 2; i++)
    {
        _displayBuffer[_rows - 1][i] = '-';
    }
    _displayBuffer[_rows - 1][_columns - 2] = '+';
    _displayBuffer[_rows - 1][_columns - 1] = '\0';
}

// Display the plotter to console.
void plotter_render()
{
    for (size_t i = 0; i < _rows; i++)
    {
        printf("%s\n", _displayBuffer[i]);
    }
    printf("\n\n");
}

void plotter_dispose()
{
    free(_displayBuffer);
}