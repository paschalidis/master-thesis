#ifndef MY_READ_FILE_H
#define MY_READ_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Read the file with data and store them to Two-Dimensional array 
 */
void readFile(const char *fileName, double **array);

/**
 * Read the file with data and count the Rows and Columns 
 * to allocate memory
 */
void countRowsCols(const char *fileName, int *rows, int *cols);
#endif