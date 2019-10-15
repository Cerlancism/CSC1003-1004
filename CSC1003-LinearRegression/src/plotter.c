#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int _rows;
static int _columns;

static char **_displayBuffer;

static void printText(char *text, unsigned int posX, unsigned int posY)
{
    if (posY > _rows - 1)
    {
        return;
    }

    int textLenth = strlen(text);
    char *line = _displayBuffer[posY];
    for (int i = 0; i < textLenth && (posX + i) < _columns; i++)
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

// Initialise the plotter display size.
void plotter_init(int rows, int colums, int xStart, unsigned int xLength, int yStart, unsigned int yLength)
{
    printf("Init Plotter\n\n\n");

    _rows = rows;
    _columns = colums;

    int leftPad = 10;

    // Allocate memory for display buffer.
    _displayBuffer = malloc(sizeof(char *) * _rows);
    char *emptyfill = charRepeat(' ', _columns);
    for (int i = 0; i < _rows; i++)
    {
        _displayBuffer[i] = malloc(sizeof(char) * (_columns + 1));
        printText(emptyfill, 0, i);
        _displayBuffer[i][_columns] = '\0';
    }
    free(emptyfill);

    // Initialise an ASCII art box
    char *topBorder = charRepeat('_', _columns - 2 - leftPad);
    char *bottomBorder = charRepeat('-', _columns - 2 - leftPad);

    printText(topBorder, 1 + leftPad, 0);

    for (int y = 1; y < _rows - 1; y++)
    {
        printText("|", leftPad, y);
        printText("|", _columns - 1, y);

        char counterPrint[3];
        sprintf(counterPrint, "%d", _rows - 2 - y);
        printText(counterPrint, 1, y);
    }

    printText(topBorder, 1 + leftPad, _rows - 2);
    free(topBorder);
    free(bottomBorder);

    printText("test", _columns - 2, -100);
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