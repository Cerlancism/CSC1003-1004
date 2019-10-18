// Classic include guard
#ifndef PLOTTER_H
#define PLOTTER_H

extern void plotter_init(int rows, int colums, float xStart, float xLength, float yStart, float yLength);
extern void plotter_printCoord(char *text, float x, float y);
extern void plotter_render();
extern void plotter_dispose();

#endif