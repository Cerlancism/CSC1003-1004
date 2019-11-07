/* Classic include guard */
#ifndef CONSOLEPLOTTER_H
#define CONSOLEPLOTTER_H

extern void consoleplotter_init(unsigned int rows, unsigned int colums, float xStart, float xLength, float yStart, float yLength);
extern void consoleplotter_printText(char *text, unsigned int posX, unsigned int posY);
extern void consoleplotter_printCoord(char *text, const float *const x, const float *const y);
extern void consoleplotter_render();
extern void consoleplotter_dispose();
extern void consoleplotter_clear();

#endif
