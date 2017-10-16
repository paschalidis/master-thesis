#include "k-means.h"

void fillData(int **array, int nRows, int nCols)
{
    int row, column;
    for (row = 0; row < nRows; row++)
    {
        for (column = 0; column < nCols; column++)
        {
            array[row][column] = 1;
        }
    }
}
