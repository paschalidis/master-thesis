#include <stdio.h>
#include <stdlib.h>

#include "read_file.h"
#include "k-means.h"

void argumentsValidator(int numberOfArguments);
void printData(double **array, int *nRows, int *nCols, int *cluster, int *k);

int main(int argc, const char *argv[])
{
    printf("MAin\n");
    argumentsValidator(argc);

    int k = atoi(argv[1]);
    const char *fileName = argv[2];

    int numberOfRows, numberOfColumns;
    double *arrayPointer;
    double **rowPointer;

    countRowsCols(fileName, &numberOfRows, &numberOfColumns);

    printf("Rows: %d\nCols: %d\n", numberOfRows, numberOfColumns);

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
    int i;
    for (i = 0; i < numberOfRows; i++)
    {
        rowPointer[i] = arrayPointer + (i * numberOfColumns);
    }

    readFile(fileName, rowPointer);

    int *clusters;
    clusters = run(rowPointer, &numberOfRows, numberOfColumns, k);

    // done: refactor print data to print per cluster and points
    printData(rowPointer, &numberOfRows, &numberOfColumns, clusters, &k);
    return 0;
}

/**
 * Validate the number of arguments 
 */
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

void printData(double **array, int *nRows, int *nCols, int *clusters, int *k)
{
    int row, column, cluster, clusterNumber;

    for(cluster = 1; cluster <= *k; cluster++)
    {
        printf("============ Cluster %d ===========\n", cluster);
        for (row = 0; row < *nRows; row++)
        {
            if(clusters[row] == cluster)
            {
                for (column = 0; column < *nCols; column++)
                {
                    printf("%lf ", array[row][column]);
                }
                printf("\n");
            }
        }        
    }

    printf("============ All Data =========\n");
    for (row = 0; row < *nRows; row++)
    {
        for (column = 0; column < *nCols; column++)
        {
            printf("%lf ", array[row][column]);
        }
        printf("\n");
    }
}