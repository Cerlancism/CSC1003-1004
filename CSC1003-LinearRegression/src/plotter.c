#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LEFT_PAD 10

#define Plot_X(x) (int)(LEFT_PAD + 1 + roundf(x))
#define Plot_Y(x) (int)(_bufferRows - 2 - roundf(x))

static int _bufferRows;
static int _bufferColumns;
static int _plotRows;
static int _plotColumns;

static char **_displayBuffer;

static void printText(char *text, unsigned int posX, unsigned int posY)
{
    if (posY > _bufferRows - 1)
    {
        return;
    }

    int textLenth = strlen(text);
    char *line = _displayBuffer[posY];
    for (int i = 0; i < textLenth && (posX + i) < _bufferColumns; i++)
    {
        line[posX + i] = text[i];
    }
}

static char *charRepeat(char character, unsigned int count)
{
    char *output = malloc(sizeof(char) * (count + 1));
    output[count] = '\0';
    for (int i = 0; i < count; i++)
    {
        output[i] = character;
    }
    return output;
}

// Initialise the plotter display size. xLength and yLength to be positive.
void plotter_init(int rows, int colums, float xStart, float xLength, int yStart, float yLength)
{
    printf("Init Plotter\n\n\n");

    if (xLength < 0 || yLength < 0)
    {
        printf("EXCEPTION: xLength and yLength must be positive.\n");
        exit(1); // Exception occurs and kills program.
    }

    _bufferRows = rows + 3;
    _bufferColumns = colums + LEFT_PAD + 3;
    _plotRows = rows;
    _plotColumns = colums;

    // Allocate memory for display buffer.
    _displayBuffer = malloc(sizeof(char *) * _bufferRows);
    char *emptyfill = charRepeat(' ', _bufferColumns);
    for (int i = 0; i < _bufferRows; i++)
    {
        _displayBuffer[i] = malloc(sizeof(char) * (_bufferColumns + 1));
        printText(emptyfill, 0, i);
        _displayBuffer[i][_bufferColumns] = '\0';
    }
    free(emptyfill);

    // Initialise an ASCII art box
    char *topBorder = charRepeat('_', _bufferColumns - 2 - LEFT_PAD);

    printText(topBorder, 1 + LEFT_PAD, 0);

    for (int y = 1; y < _bufferRows - 1; y++)
    {
        printText("|", LEFT_PAD, y);
        printText("|", _bufferColumns - 1, y);

        char lablePrint[8];
        sprintf(lablePrint, "%7.4f", (float)_bufferRows - 2 - y);
        printText(lablePrint, 1, y);
    }

    printText(topBorder, 1 + LEFT_PAD, _bufferRows - 2);

    for (int i = 0; (i + 1) < _plotColumns; i += 10)
    {
        char lablePrint[4];
        sprintf(lablePrint, "|%.2f", (float)i);
        printText(lablePrint, Plot_X(i), _bufferRows - 1);
    }

    for (int x = 0; x < _plotColumns; x++)
    {
        printText("*", Plot_X(x), Plot_Y(-0.0075 * pow(x, 2) + 0.5 * x + 40));
    }

    free(topBorder);

    printText("test", _bufferColumns - 2, -100);
}

// Display the plotter to console.
void plotter_render()
{
    for (size_t i = 0; i < _bufferRows; i++)
    {
        printf("%s\n", _displayBuffer[i]);
    }
    printf("\n\n");
}

void plotter_dispose()
{
    free(_displayBuffer);
}