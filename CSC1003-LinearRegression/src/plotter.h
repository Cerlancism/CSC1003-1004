// Classic include guard
#ifndef PLOTTER_H
#define PLOTTER_H

extern void plotter_init(int rows, int colums, int xStart, unsigned int xLength, int yStart, unsigned int yLength);
extern void plotter_render();
extern void plotter_dispose();

#endif