// Classic include guard
#ifndef PLOTTER_H
#define PLOTTER_H

void plotter_init(int rows, int colums, int xStart, unsigned int xLength, int yStart, unsigned int yLength);
void plotter_render();
void plotter_dispose();

#endif