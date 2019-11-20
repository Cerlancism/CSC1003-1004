/* Classic include guard */
#ifndef GNUPLOTTER_H
#define GNUPLOTTER_H

extern int gnuplotter_exists();
extern FILE *gnuplotter_pipe(const char *datafile, const char *histogramFile, const char *binFile, float m, float c, float gaussianBase, float mean, float sd, float multiplier, int partitions);

#endif
