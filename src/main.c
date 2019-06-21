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

/**
 * Validate K number
 * @param k
 * @param numberOfRows
 */
void validateK(int const *k, int const *numberOfRows);

int main(int argc, const char *argv[])
{
    //used it to generate new random number each time
    srand( (unsigned int) time(NULL) );

    argumentsValidator(argc);

    // Fill arguments to variables
    const int k = atoi(argv[K_ARGUMENT_INDEX]);
    const char *fileName = argv[FILE_NAME_ARGUMENT_INDEX];
    const char *separator = argv[SEPARATOR_ARGUMENT_INDEX];
    if(separator == NULL){
        separator = DEFAULT_SEPARATOR;
    }

    // Points
    int numberOfRows, numberOfColumns;
    double *arrayPointer;
    double **rowPointer;

    // centers of k-means
    double *centerPointer;
    double **centers;

    // Results
    int *clusters;
    double *centerRadius;

    countRowsCols(fileName, &numberOfRows, &numberOfColumns, separator);

    validateK(&k, &numberOfRows);
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

    // allocate the memory for the center pointers
    centerPointer = malloc(k * numberOfColumns * sizeof(double));
    if (centerPointer == NULL)
    {
        printf("\nFailure to allocate room for the center pointers");
        exit(0);
    }
    // allocate room for the pointers to the centers
    centers = malloc(k * sizeof(double *));
    if (centers == NULL)
    {
        printf("\nFailure to allocate room for pointers");
        exit(0);
    }
    // point the pointers for centers
    for (int i = 0; i < k; i++)
    {
        centers[i] = centerPointer + (i * numberOfColumns);
    }

    // allocate memory for radius
    centerRadius = malloc(k * sizeof(double *));
    if (centerRadius == NULL) {
        printf("\nFailure to allocate room for the centerRadius2");
        exit(0);
    }

    // initialize center radius
    for(int i = 0; i < k; i++){
        centerRadius[i] = 0;
    }

    readFile(fileName, rowPointer, separator);

    clusters = run(rowPointer, &numberOfRows, &numberOfColumns, &k, centers, centerRadius);

    for(int i = 0; i < k; i++){
        printf("\nCenter centerRadius\n");
        printf("Radius of Center %d is %f\n",i, centerRadius[i]);
    }


    writeClusters(rowPointer, &numberOfRows, &numberOfColumns, clusters, &k);
    writeCenters(&numberOfColumns, &k, centers, centerRadius);

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

void validateK(int const *k, int const *numberOfRows)
{
    if(*k < 1){
        printf("K must be bigger than 1\n");
        exit(0);
    }
    if(*k >= *numberOfRows){
        printf("K must be lower than items: %d \n", *numberOfRows);
        exit(0);
    }
}