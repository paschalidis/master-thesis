#include <stdio.h>
#include <stdlib.h>

void fillData(int **array,  int nRow, int nCols);
void printData(int **array, int nRows, int nCols);

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

    printf("\n\nIndex\tPOinter(hex)\tDiff.(dec)");

    for (row = 0; row < nrows; row++)
    {
        printf("\n%d\t%p", row, rptr[row]);
        if (row > 0)
        {
            printf("\t%ld",(rptr[row] - rptr[row - 1]));
        }
    }
    fillData(rptr, nrows, ncols);
    printf("\nPrinte the array \n");
    printData(rptr, nrows, ncols);
    return 0;
}

void fillData(int **array, int nRows, int nCols){
    int row, column;
    for (row = 0; row < nRows; row++)
    {
        for (column = 0; column < nCols; column++)
        {
            array[row][column] = 1;
        }
    }
}

void printData(int **array, int nRows, int nCols)
{
    int row, column;
    for (row = 0; row < nRows; row++)
    {
        for (column = 0; column < nCols; column++)
        {
            printf("%d ", array[row][column]);
        }
        printf("\n");
    }
}