/*

plotter.c
Treats the console out like a canvas drawing ascii characters on position x and y.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mathsFallback.h"

#define LEFT_PAD 10

/* Offset x buffer position relative the plot */
#define Print_Coord_X(x) (int)(LEFT_PAD + 1 + fallback_roundf(x))
/* Offset y buffer position relative the plot */
#define Print_Coord_Y(x) (int)(_bufferRows - 2 - fallback_roundf(x))

static unsigned int _bufferRows;
static unsigned int _bufferColumns;
static unsigned int _plotRows;
static unsigned int _plotRows;
static unsigned int _plotColumns;
static float _xMultiplier;
static float _xOffset;
static float _yMultiplier;
static float _yOffset;

static char **_displayBuffer;

static char *charRepeat(char character, unsigned int count)
{
    size_t i;
    char *output = malloc(sizeof(char) * (count + 1));
    output[count] = '\0';
    for (i = 0; i < count; i++)
    {
        output[i] = character;
    }
    return output;
}

/* Prints a string of text on the plotter view port */
static void printText(char *text, unsigned int posX, unsigned int posY)
{
    int textLength;
    int i;
    char *line;

    if (posY > _bufferRows - 1)
    {
        return;
    }

    textLength = strlen(text);
    line = _displayBuffer[posY];
    for (i = 0; i < textLength && (posX + i) < _bufferColumns; i++)
    {
        line[posX + i] = text[i];
    }
}

/* Prints a string on the plotter coordinate. */
void plotter_printCoord(char *text, const float *const x, const float *const y)
{
    int plotX = Print_Coord_X((*x + _xOffset) / _xMultiplier);
    int plotY = Print_Coord_Y((*y + _yOffset) / _yMultiplier);

    if ((plotY > 0) && (plotY <= ((int)_plotRows + 1)) && (plotX > LEFT_PAD))
    {
        printText(text, plotX, plotY);
    }
}

/* Initialise the plotter display size. xLength and yLength to be positive. */
void plotter_init(unsigned int rows, unsigned int colums, float xStart, float xLength, float yStart, float yLength)
{
    char *emptyfill, *topBorder;
    size_t i, y, x;
    puts("");

    if (xLength < 0 || yLength < 0)
    {
        puts("EXCEPTION: xLength and yLength must be positive.");
        exit(0); /* Exception occurs and kills program. */
    }

    _bufferRows = rows + 3;
    _bufferColumns = colums + LEFT_PAD + 3;
    _plotRows = rows;
    _plotColumns = colums;

    _xOffset = -xStart;
    _yOffset = -yStart;
    _xMultiplier = xLength / colums;
    _yMultiplier = yLength / rows;

    /* Allocate memory for display buffer. */
    _displayBuffer = malloc(sizeof(char *) * _bufferRows);
    emptyfill = charRepeat(' ', _bufferColumns);
    for (i = 0; i < _bufferRows; i++)
    {
        _displayBuffer[i] = malloc(sizeof(char) * (_bufferColumns + 1));
        printText(emptyfill, 0, i);
        _displayBuffer[i][_bufferColumns] = '\0';
    }
    free(emptyfill);

    /* Initialise an ASCII art box */
    topBorder = charRepeat('_', _bufferColumns - 2 - LEFT_PAD);

    printText(topBorder, 1 + LEFT_PAD, 0);

    /* Prints the y bar labelling */
    for (y = 1; y < _bufferRows - 1; y++)
    {
        char labelPrint[10];
        printText("|", LEFT_PAD, y);
        printText("|", _bufferColumns - 1, y);
        sprintf(labelPrint, "%7.3f", ((_bufferRows - 2 - y) * _yMultiplier - _yOffset));
        printText(labelPrint, 1, y);
    }

    printText(topBorder, 1 + LEFT_PAD, _bufferRows - 2);

    /* Prints the x bar labelling */
    for (i = 0; (i + 1) < _plotColumns; i += 10)
    {
        char labelPrint[10];
        sprintf(labelPrint, "|%-7.3f", i * _xMultiplier + xStart);
        printText(labelPrint, Print_Coord_X(i), _bufferRows - 1);
    }

    /* Draw the x axis */
    for (x = 0; x < _plotColumns; x++)
    {
        printText("-", Print_Coord_X(x), Print_Coord_Y(_yOffset / _yMultiplier));
    }

    /* Draw the y axis */
    if (_xOffset > 0)
    {
        for (y = 0; y <= _plotRows; y++)
        {
            printText("|", Print_Coord_X(_xOffset / _xMultiplier), Print_Coord_Y(y));
        }
    }

    free(topBorder);
}

/* Display the plotter to console. */
void plotter_render()
{
    size_t i;
    for (i = 0; i < _bufferRows; i++)
    {
        puts(_displayBuffer[i]);
    }
    puts("");
}

void plotter_dispose()
{
    size_t i;
    for (i = 0; i < _bufferRows; i++)
    {
        free(_displayBuffer[i]);
    }
    free(_displayBuffer);
}
