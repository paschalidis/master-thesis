#ifndef MY_K_MEANS_H
#define MY_K_MEANS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 *
 * @param points
 * @param rows
 * @param dimensions
 * @param k
 * @param centers
 * @return
 */
int *run(double **points, int const *rows, int *dimensions, int *k, double **centers);

/**
 * Calculate radius for each center (max distance)
 *
 * @param k
 * @param points
 * @param centers
 * @param clusters
 * @param rows
 * @param dimensions
 * @return
 */
double *
radius(int const *k, double **points, double **centers, int const *clusters, const int *rows, const int *dimensions);

#endif
