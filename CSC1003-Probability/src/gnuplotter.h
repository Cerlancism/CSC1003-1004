/* Classic include guard */
#ifndef GNUPLOTTER_H
#define GNUPLOTTER_H

extern int gnuplotter_exists();
extern FILE *gnuplotter_pipe(const char *datafile, const char *histogramFile, const char *binsFile, float m, float c, float gaussianBase, float mean, float sd, float magnitude, int partitions, float noiseMin, float noiseMax);

#endif
