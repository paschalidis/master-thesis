#include "file.h"

void readFile(const char *fileName, double **array, const char *separator) {
    char buffer[1024];
    char *record, *line;
    int i = 0, j = 0;

    FILE *fstream = fopen(fileName, "r");
    if (fstream == NULL) {
        printf("\n file opening failed ");
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
    char buffer[1024];
    char *record, *line;
    int row = 0, column = 0;

    FILE *fstream = fopen(fileName, "r");
    if (fstream == NULL) {
        printf("\n file opening failed ");
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

void writeClusters(double **array, int const *nRows, int const *nCols, int const *clusters, int const *k) {
    char clusterStr[9] = "cluster_";
    char fileName[10], clusterNumber[2];
    int row, column, cluster;

    // Clusters
    for (cluster = 0; cluster < *k; cluster++) {
        itoa(cluster + 1, clusterNumber, 10);
        strcpy(fileName, clusterStr);
        strcat(fileName, clusterNumber);
        FILE *fp;
        fp = fopen(fileName, "w");
        if (fp == NULL) {
            printf("\n file to write results opening failed ");
            exit(0);
        }

        for (row = 0; row < *nRows; row++) {
            if (clusters[row] == cluster) {
                for (column = 0; column < *nCols; column++) {
                    fprintf(fp, "%lf ", array[row][column]);
                }
                fprintf(fp, "\n");
            }
        }
        fclose(fp);
    }
}

//todo write radius for each center(max distance)
void writeCenters(int const *nCols, int const *k, double **centers) {

    // Centers
    FILE *fp;
    fp = fopen("centers", "w");
    if (fp == NULL) {
        printf("\n file to write centers opening failed ");
        exit(0);
    }

    for (int i = 0; i < *k; i++) {
        for (int column = 0; column < *nCols; column++) {
            fprintf(fp, "%lf ", centers[i][column]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}
