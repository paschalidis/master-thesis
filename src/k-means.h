//#ifndef KMEANS_H
//#define KMEANS_H
//#include <stdio.h>
//void fillData(int **array,  int nRow, int nCols);
void initializeClusters(int *clusters, int *rows);

int * run(double **pointers, int *rows, int dementions, int k)
{
    int *clusters;
    double **centros;

    // allocate memory for clusters
    clusters = malloc((*rows) * sizeof(int));
    initializeClusters(clusters, rows);

    // todo: init ceentros
    // todo: allocate memory and pointers for centros
    // todo: randomize centros

    // todo: Do Kmeans algortithms
    // todo: function for evlikia apostasi
    // todo: average per cluster to calclulate new centros

    return clusters;
}

void initializeClusters(int *clusters, int *rows)
{
    int i;
    for(i =0; i < *rows; i++){
        clusters[i] = 1;
    }
}

//#endif