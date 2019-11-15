/*

consoleplotter.c
Treats the console out like a canvas drawing ascii characters on position x and y.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "mathsUtils.h" /* roundf */

#define LEFT_PAD 10

/* Offset x buffer position relative the plot */
#define Print_Coord_X(x) (int)(LEFT_PAD + 1 + roundf(x))
/* Offset y buffer position relative the plot */
#define Print_Coord_Y(x) (int)(_bufferRows - 2 - roundf(x))

#define BORDER_LEN (_bufferColumns - 2 - LEFT_PAD)

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

/* Prints a string of text on the plotter view port */
void consoleplotter_printText(char *text, unsigned int posX, unsigned int posY)
{
    char *line;
    int textLength;
    int i;

    if (posY > _bufferRows - 1)
    {
        return;
    }

    line = _displayBuffer[posY];

    if (posX < _bufferColumns && *(text + 1) == '\0') /* if just one character to print */
    {
        line[posX] = text[0];
    }
    else
    {
        textLength = strlen(text);
        for (i = 0; i < textLength && (posX + i) < _bufferColumns; i++)
        {
            line[posX + i] = text[i];
        }
    }
}

/* Prints a string on the plotter coordinate. */
void consoleplotter_printCoord(char *text, const float *const x, const float *const y)
{
    int plotX = Print_Coord_X((*x + _xOffset) / _xMultiplier);
    int plotY = Print_Coord_Y((*y + _yOffset) / _yMultiplier);

    if ((plotY > 0) && (plotY <= ((int)_plotRows + 1)) && (plotX > LEFT_PAD) && plotX < ((int)_bufferColumns - 1))
    {
        consoleplotter_printText(text, plotX, plotY);
    }
}

/* Initialise the plotter display size. xLength and yLength to be positive. */
void consoleplotter_init(unsigned int rows, unsigned int colums, float xStart, float xLength, float yStart, float yLength)
{
    size_t x, y;
    char *border;

    puts("");

    if (xLength < 0 || yLength < 0)
    {
        puts("EXCEPTION: xLength and yLength must be positive.");
        exit(1); /* Exception occurs and kills program. */
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
    if (!_displayBuffer)
    {
        _displayBuffer = malloc(sizeof(char *) * _bufferRows);

        if (!_displayBuffer)
        {
            puts("MEMORY FAILURE: Failed allocating display buffer!");
            exit(1); /* Exception occurs and kills program. */
        }

        for (y = 0; y < _bufferRows; y++)
        {
            _displayBuffer[y] = malloc(sizeof(char) * (_bufferColumns + 1));

            if (!_displayBuffer[y])
            {
                puts("MEMORY FAILURE: Failed allocating display buffer line!");
                exit(1); /* Exception occurs and kills program. */
            }

            memset(_displayBuffer[y], ' ', (sizeof(char) * (_bufferColumns)));
            _displayBuffer[y][_bufferColumns] = '\0';
        }
    }

    /* Initialise an ASCII art box */
    border = malloc(sizeof(char) * (BORDER_LEN + 1));
    memset(border, '_', sizeof(char) * (BORDER_LEN));
    border[BORDER_LEN] = '\0';

    consoleplotter_printText(border, 1 + LEFT_PAD, 0);

    /* Prints the y bar labelling */
    for (y = 1; y < _bufferRows - 1; y++)
    {
        char labelPrint[10];
        consoleplotter_printText("|", LEFT_PAD, y);
        consoleplotter_printText("|", _bufferColumns - 1, y);
        sprintf(labelPrint, "%7.3f", ((_bufferRows - 2 - y) * _yMultiplier - _yOffset));
        consoleplotter_printText(labelPrint, 1, y);
    }

    consoleplotter_printText(border, 1 + LEFT_PAD, _bufferRows - 2);

    /* Prints the x bar labelling */
    for (x = 0; (x + 1) < _plotColumns; x += 10)
    {
        char labelPrint[10];
        sprintf(labelPrint, "|%-7.3f", x * _xMultiplier + xStart);
        consoleplotter_printText(labelPrint, Print_Coord_X(x), _bufferRows - 1);
    }

    if ((Print_Coord_Y(_yOffset / _yMultiplier) - (int)_bufferRows + 2) < 0)
    {
        /* Draw the x axis */
        for (x = 0; x < _plotColumns; x++)
        {
            consoleplotter_printText("-", Print_Coord_X(x), Print_Coord_Y(_yOffset / _yMultiplier));
        }
    }

    /* Draw the y axis */
    if (_xOffset > 0)
    {
        for (y = 0; y <= _plotRows; y++)
        {
            consoleplotter_printText("|", Print_Coord_X(_xOffset / _xMultiplier), Print_Coord_Y(y));
        }
    }

    free(border);
}

/* Display the plotter to console. */
void consoleplotter_render()
{
    size_t i;
    for (i = 0; i < _bufferRows; i++)
    {
        puts(_displayBuffer[i]);
    }
    puts("");
}

void consoleplotter_clear()
{
    size_t i;
    for (i = 0; i < _bufferRows; i++)
    {
        memset(_displayBuffer[i], ' ', sizeof(char) * _bufferColumns);
    }
}

void consoleplotter_dispose()
{
    size_t i;
    for (i = 0; i < _bufferRows; i++)
    {
        free(_displayBuffer[i]);
    }
    free(_displayBuffer);

    _displayBuffer = NULL;
}
