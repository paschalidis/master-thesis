#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"

int main(int argc, const char *argv[])
{
    int randomNumber;
    int N;
    int range;
    char clusterStr[13] = "random_data_";
    char fileName[10], clusterNumber[2];
    //used it to generate new random number each time
    srand( (unsigned int) time(NULL) );

    printf("Give N:");
    scanf("%d", &N);
    printf("Give NRandom Range:");
    scanf("%d", &range);

    sprintf(clusterNumber, "%d", N);
    strcpy(fileName, clusterStr);
    strcat(fileName, clusterNumber);

    FILE *fp;
    fp = fopen(fileName, "w");

    for(int i =0; i < N; i++){
        randomNumber = rand() % range;
        fprintf(fp, "%d\n", randomNumber);
//        printf("%d\n", randomNumber);
    }

    fclose(fp);

    return 0;
}