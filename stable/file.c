#include "file.h"

void readFile(const char *fileName, double **array, const char *separator) {
    char buffer[FILE_BUFFER_LENGTH];
    char *record, *line;
    int i = 0, j = 0;

    FILE *fstream = fopen(fileName, "r");
    if (fstream == NULL) {
        printf("\n file %s opening failed ", fileName);
        exit(0);
    }

    while ((line = fgets(buffer, sizeof(buffer), fstream)) != NULL) {
        record = strtok(line, separator);
        while (record != NULL) {
            array[i][j++] = atof(record);
            record = strtok(NULL, separator);
        }
        j = 0;
        i++;
    }
    fclose(fstream);
}

void countRowsCols(const char *fileName, int *rows, int *cols, const char *separator) {
    char buffer[FILE_BUFFER_LENGTH];
    char *record, *line;
    int row = 0, column = 0;

    FILE *fstream = fopen(fileName, "r");
    if (fstream == NULL) {
        printf("\n file %s opening failed ", fileName);
        exit(0);
    }

    while ((line = fgets(buffer, sizeof(buffer), fstream)) != NULL) {
        record = strtok(line, separator);
        while (record != NULL) {
            record = strtok(NULL, separator);
            column++;
        }
        *cols = column;
        column = 0;
        row++;
    }
    *rows = row;
    fclose(fstream);
}

void writeClusters(double **array, int const *rows, int const *cols, int const *clusters, int const *k) {
    char clusterStr[9] = "cluster_";
    char fileName[12], clusterNumber[4];
    int row, column, cluster;

    // Clusters
    for (cluster = 0; cluster < *k; cluster++) {
        // Create file name eg. cluster_1
        sprintf(clusterNumber, "%d", cluster + 1);
        strcpy(fileName, clusterStr);
        strcat(fileName, clusterNumber);

        FILE *fp;
        fp = fopen(fileName, "w");
        if (fp == NULL) {
            printf("\n file %s opening failed ", fileName);
            exit(0);
        }

        // Export cluster
        for (row = 0; row < *rows; row++) {
            if (clusters[row] == cluster) {
                for (column = 0; column < *cols; column++) {
                    fprintf(fp, "%lf ", array[row][column]);
                }
                fprintf(fp, "\n");
            }
        }
        fclose(fp);
    }
}

void writeCenters(int const *cols, int const *k, double **centers, double *centerRadius) {

    // Centers
    FILE *fp;
    fp = fopen(FILE_NAME_CENTERS, "w");
    if (fp == NULL) {
        printf("\n file %s opening failed ", FILE_NAME_CENTERS);
        exit(0);
    }

    for (int i = 0; i < *k; i++) {
        fprintf(fp, "Center %d Radius: %lf \n", i + 1, centerRadius[i]);
        fprintf(fp, "Point: ");
        for (int column = 0; column < *cols; column++) {
            fprintf(fp, "%lf ", centers[i][column]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

void writeResults(int const *k, double time, char const *method, int const *rows, int const *cols, const char *fileName, int const *iterations, int numberOfThreads)
{
    FILE *fp, *fplog;
    fp = fopen(FILE_NAME_RESULTS, "w");
    fplog = fopen(FILE_NAME_RESULTS_LOG, "a");
    if (fp == NULL) {
        printf("\n file %s opening failed ", FILE_NAME_RESULTS);
        exit(0);
    }
    if (fplog == NULL) {
        printf("\n file %s opening failed ", FILE_NAME_RESULTS_LOG);
        exit(0);
    }
    fprintf(fp, "K-means = %s\tThreads = %d\tK = %d\tN = %d\tDimension = %d\tIterations = %d\tTime = %f sec\t\tFile = %s \n", method, numberOfThreads, *k, *rows, *cols, *iterations, time, fileName);
    fprintf(fplog, "K-means = %s\tThreads = %d\tK = %d\tN = %d\tDimension = %d\tIterations = %d\tTime = %f sec\t\tFile = %s \n", method, numberOfThreads, *k, *rows, *cols, *iterations, time, fileName);

    fclose(fp);
    fclose(fplog);
}

int fileExist(const char *fileName)
{
    if( access( fileName, F_OK ) != -1 ) {
        return 1;
    } else {
        return 0;
    }
}

void readRandomRowFile(const char *fileName, int *array)
{
    char buffer[FILE_BUFFER_LENGTH];
    char *line, *record;
    int i = 0;

    FILE *fstream = fopen(fileName, "r");
    if (fstream == NULL) {
        printf("\n file %s opening failed ", fileName);
        exit(0);
    }

    while ((line = fgets(buffer, sizeof(buffer), fstream)) != NULL) {
        record = strtok(line, "");
        array[i] = atoi(record);
        i++;
    }
    fclose(fstream);
}

void writeRandomRowFile(int const *array, const char *fileName, int const *k)
{
    FILE *fp;
    fp = fopen(fileName, "w");
    if (fp == NULL) {
        printf("\n file %s opening failed ", fileName);
        exit(0);
    }

    for(int i = 0; i < *k; i ++){
        fprintf(fp, "%d\n", array[i]);
    }

    fclose(fp);
}