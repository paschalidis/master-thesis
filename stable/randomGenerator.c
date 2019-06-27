#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"

int main(int argc, const char *argv[])
{
    int randomNumber;
    int N;
    int range;
    int dimension;
    char fileStr[13] = "random_data_";
    char fileName[30], rows[2], columns[2];
    //used it to generate new random number each time
    srand( (unsigned int) time(NULL) );

    printf("Give N:");
    scanf("%d", &N);
    printf("Give Random Range:");
    scanf("%d", &range);
    printf("Give Dimension:");
    scanf("%d", &dimension);

    sprintf(rows, "%d", N);
    sprintf(columns, "%d", dimension);
    strcpy(fileName, fileStr);
    strcat(fileName, rows);
    strcat(fileName, "_");
    strcat(fileName, columns);

    FILE *fp;
    fp = fopen(fileName, "w");

    for(int i =0; i < N; i++){
        for(int j = 0; j < dimension; j++){
            randomNumber = rand() % range;
            if(j == (dimension - 1)){
                fprintf(fp, "%d", randomNumber);
            } else {
                fprintf(fp, "%d ", randomNumber);
            }
        }
        fprintf(fp, "\n");
//        printf("%d\n", randomNumber);
    }

    fclose(fp);

    return 0;
}