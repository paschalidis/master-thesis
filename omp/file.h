#ifndef MY_FILE_H
#define MY_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME_CENTERS "centers"
#define FILE_BUFFER_LENGTH 1024

/**
 * Read the file with data and store them to Two-Dimensional array
 *
 * @param fileName
 * @param array
 * @param separator
 */
void readFile(const char *fileName, double **array, const char *separator);

/**
 * Read the file with data and count the Rows and Columns 
 * to allocate memory
 *
 * @param fileName
 * @param rows
 * @param cols
 * @param separator
 */
void countRowsCols(const char *fileName, int *rows, int *cols, const char *separator);

/**
 * Export each clusters in separate file
 *
 * @param array
 * @param rows
 * @param cols
 * @param clusters
 * @param k
 */
void writeClusters(double **array, int const *rows, int const *cols, int const *clusters, int const *k);

/**
 * Export centers and radius
 *
 * @param cols
 * @param k
 * @param centers
 * @param centerRadius
 */
void writeCenters(int const *cols, int const *k, double **centers, double *centerRadius);

#endif