#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "k-means.h"
#include "time.h"

#define DEFAULT_SEPARATOR " "
#define K_ARGUMENT_INDEX 1
#define FILE_NAME_ARGUMENT_INDEX 2
#define SEPARATOR_ARGUMENT_INDEX 3

/**
 * Check arguments pass it from command line for execution
 * @param numberOfArguments
 */
void argumentsValidator(int numberOfArguments);

int main(int argc, const char *argv[])
{
    //used it to generate new random number each time
    srand( (unsigned int) time(NULL) );

    argumentsValidator(argc);

    // Fill arguments to variables
    int k = atoi(argv[K_ARGUMENT_INDEX]);
    if(k < 1){
        printf("K must be bigger than 1\n");
        exit(0);
    }
    const char *fileName = argv[FILE_NAME_ARGUMENT_INDEX];
    const char *separator = argv[SEPARATOR_ARGUMENT_INDEX];
    if(separator == NULL){
        separator = DEFAULT_SEPARATOR;
    }

    int numberOfRows, numberOfColumns;
    double *arrayPointer;
    double **rowPointer;

    countRowsCols(fileName, &numberOfRows, &numberOfColumns, separator);

    printf("Rows: %d\tCols: %d\n", numberOfRows, numberOfColumns);

    // allocate the memory for the array
    arrayPointer = malloc(numberOfRows * numberOfColumns * sizeof(double));
    if (arrayPointer == NULL)
    {
        printf("\nFailure to allocate room for the array");
        exit(0);
    }
    // allocate room for the pointers to the rows
    rowPointer = malloc(numberOfRows * sizeof(double *));
    if (rowPointer == NULL)
    {
        printf("\nFailure to allocate room for pointers");
        exit(0);
    }
    // point the pointers
    for (int i = 0; i < numberOfRows; i++)
    {
        rowPointer[i] = arrayPointer + (i * numberOfColumns);
    }

    readFile(fileName, rowPointer, separator);

    int *clusters;
    clusters = run(rowPointer, &numberOfRows, &numberOfColumns, &k);

    writeResults(rowPointer, &numberOfRows, &numberOfColumns, clusters, &k);
    return 0;
}

void argumentsValidator(int numberOfArguments)
{
    if (numberOfArguments < 3)
    {
        printf("Error on arguments\n");
        printf("Usage: ./main K filePath\n");
        printf("Example: ./main 3 data/sample.csv\n");
        exit(0);
    }
}