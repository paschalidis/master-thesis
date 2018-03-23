#ifndef MY_READ_FILE_H
#define MY_READ_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//todo define default separator
/**
 * Read the file with data and store them to Two-Dimensional array 
 */
void readFile(const char *fileName, double **array, const char *separator);

/**
 * Read the file with data and count the Rows and Columns 
 * to allocate memory
 */
void countRowsCols(const char *fileName, int *rows, int *cols, const char *separator);
#endif