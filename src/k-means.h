//#ifndef KMEANS_H
//#define KMEANS_H
//#include <stdio.h>
//void fillData(int **array,  int nRow, int nCols);
void initializeClusters(int *clusters, int *rows);
void randomizeCenters(double *centers, int *k);

int * run(double **points, int *rows, int dimensions, int *k)
{
    // one dimensional array to store the cluster of each data row
    int *clusters;

    // centers of k-means
    double *centers;

    // allocate memory for clusters
    clusters = malloc((*rows) * sizeof(int));
    initializeClusters(clusters, rows);

    // allocate memory for centers
    centers = malloc((*k) * sizeof(double));
    randomizeCenters(centers, k);

    // todo: Do Kmeans algortithms
    // todo: function for evlikia apostasi
    // todo: average per cluster to calclulate new centros
    
    // K-means steps
    // While you have change in clusters continue
    // calculate for each point Euclidean distance from centers
        // for each row
            // for each dimension
                // for each center -- this can be seperate function
                    // calculate the distance and find the min
                    // set point to cluster
                    // if cluster has change update flag

    printf("============== Run all points of k-means =============\n");
    int i,j;
    for (i = 0; i < *rows; i++)
    {
        for (j = 0; j < dimensions; j++)
        {
           printf("%lf ", points[i][j]);
        }
        printf("\n");
    }
    printf("============== Ebd of Run all points of k-means =============\n");

    // and add them to nears cluster (min distance from center)
    // if point change cluster update the flag to continue run

    // When you add all points to relevant cluster then
    // calculate the average per cluster and set new centers
        // for each cluster
            // for each k --> the number of cluster
                // calculate average 
                // and update ne center

    // re run all steps until ther are no change to cluster


    return clusters;
}

/**
 * Set all clusters to zero value for first run
 */
void initializeClusters(int *clusters, int *rows)
{
    int i;
    for(i = 0; i < *rows; i++){
        // todo: fix it to zero after update
        clusters[i] = 1;
    }
}

/**
 * Randomize Centers of K-means for first run
 */
void randomizeCenters(double *centers, int *k)
{
    int i;
    for(i = 0; i < *k; i++)
    {
        // todo: fix random 
        centers[i] = ((double)rand()/(double)RAND_MAX);;
        printf("%f\n", centers[i]);
    }
}

//#endif