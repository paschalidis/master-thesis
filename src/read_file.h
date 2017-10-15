#ifndef MY_READ_FILE_H
#define MY_READ_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readFile(const char *fileName, double **array);
void countRowsCols(const char *fileName, int *rows, int *cols);
#endif