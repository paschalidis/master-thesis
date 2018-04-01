#ifndef MY_FILE_H
#define MY_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void writeResults(double **array, int const *nRows, int const *nCols, int const *clusters, int const *k, double **centers);

#endif