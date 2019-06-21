#ifndef MY_K_MEANS_H
#define MY_K_MEANS_H

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 * @param points
 * @param rows
 * @param dimensions
 * @param k
 * @param centers
 * @param centerRadius
 * @return
 */
int *run(double **points, int const *rows, int *dimensions, int const *k, double **centers, double *centerRadius);


#endif
