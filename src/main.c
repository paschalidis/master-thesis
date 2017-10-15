#include <stdio.h>
#include <stdlib.h>
#include "k-means.h"

int main(void)
{
    int nrows = 10;
    int ncols = 2;
    int row, col;
    int k;
    int **rptr;
    int *aptr;

    // allocate the memory for the array
    aptr = malloc(nrows * ncols * sizeof(int));
    if(aptr == NULL){
        printf("\nFailure to allocate room for the array");
        exit(0);
    }
    // allocate room for the pointers to the rows
    rptr = malloc(nrows * sizeof(int *));
    if(rptr == NULL){
        printf("\nFailure to allocate room for pointers");
        exit(0);
    }
    // point the pointers
    for(k = 0; k < nrows; k++)
    {
        rptr[k] = aptr + (k * ncols);
    }

    fillData(rptr, nrows, ncols);
    printf("\nPrinte the array \n");
    printData(rptr, nrows, ncols);
    return 0;
}