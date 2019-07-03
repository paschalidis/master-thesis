#include "k-means.h"

int *run(double **points, int const *rows, int const *dimensions, int const *k, double **centers, int *iterations, double *time) {
    // indexes of row, dimension, center(k)
    int i, j, centerIndex, totalRuns = 0;

    // one dimensional array to store the cluster of each data row
    int *clusters;

    // Distance values used to calculate Euclidean Distance
    double minDistance, sumDistance, euclideanDistance, pointDistance;

    // cluster found from Euclidean Distance
    int cluster;

    // flag to repeat k-means process as one or more cluster has change
    int clusterChange;

    // one dimensional array to store total points per cluster
    int *clusterItems;

    // centers sums per column of k-means
    double *centerSumPointer;
    double **centersSum;

    // allocate memory for clusters
    clusters = calloc(*rows, sizeof(int));
    if (clusters == NULL) {
        printf("\nFailure to allocate room for the clusters");
        exit(0);
    }

    clusterItems = calloc(*k, sizeof(int));
    if (clusterItems == NULL) {
        printf("\nFailure to allocate room for the clusters items");
        exit(0);
    }

    // allocate the memory for the center sum pointers
    centerSumPointer = calloc(*k * *dimensions, sizeof(double));
    if (centerSumPointer == NULL)
    {
        printf("\nFailure to allocate room for the center sum pointers");
        exit(0);
    }
    // allocate room for the pointers to the centers sum
    centersSum = malloc((*k) * sizeof(double *));
    //centersSum = calloc(*k, sizeof(double *));
    if (centersSum == NULL)
    {
        printf("\nFailure to allocate room for center sum");
        exit(0);
    }
    // point the pointers for centers
    for (i = 0; i < *k; i++)
    {
        centersSum[i] = centerSumPointer + (i * (*dimensions));
    }

    // K-means steps
    // While you have change in clusters continue
    // calculate for each point Euclidean distance from centers
    // for each row
    // for each dimension
    // for each center -- this can be separate function
    // calculate the distance and find the min
    // set point to cluster
    // if cluster has change update flag
    // and add them to nears cluster (min distance from center)
    // if point change cluster update the flag to continue run

    // When you add all points to relevant cluster then
    // calculate the average per cluster and set new centers
    // for each cluster
    // for each k --> the number of cluster
    // calculate average
    // and update new center
    // re run all steps until there are no change to cluster

    // Timer
    clock_t tic = clock();
    do {
        clusterChange = 0;
        for (i = 0; i < *rows; i++) {
            minDistance = -1;
            cluster = 0;
            for (centerIndex = 0; centerIndex < *k; centerIndex++) {
                sumDistance = 0;
                for (j = 0; j < *dimensions; j++) {
                    // Point distance from center
                    pointDistance = points[i][j] - centers[centerIndex][j];
                    // Power of pointDistance used to calculate Euclidean Distance
                    pointDistance = pointDistance * pointDistance;
                    // Add to sum
                    sumDistance = sumDistance + pointDistance;

                }
                euclideanDistance = sqrt(sumDistance);
                // Set first distance as min distance
                if (minDistance == -1 || euclideanDistance < minDistance) {
                    minDistance = euclideanDistance;
                    cluster = centerIndex;
                }
            }

            clusterChange = clusterChange || (clusters[i] != cluster);
            clusters[i] = cluster;

            //todo this must be local per thread
            clusterItems[cluster]++;

            for(j = 0; j < *dimensions; j++){
                //todo this must be local per thread
                centersSum[cluster][j] = centersSum[cluster][j] + points[i][j];
            }
        }

        if (clusterChange == 1) {
            //todo reduction all local thread vars to main thread

            // Average per column for new centers
            for (centerIndex = 0; centerIndex < *k; centerIndex++) {
                for (j = 0; j < *dimensions; j++) {
                    if (clusterItems[centerIndex] > 0) {
                        centers[centerIndex][j] = centersSum[centerIndex][j] / clusterItems[centerIndex];
                    } else {
                        centers[centerIndex][j] = 0;
                    }
                    centersSum[centerIndex][j] = 0;
                }
                clusterItems[centerIndex] = 0;
            }
        }
        totalRuns ++;
    } while (clusterChange == 1);

    clock_t toc = clock();
    *time = (double)(toc - tic) / CLOCKS_PER_SEC;

    free(clusterItems);
    free(centersSum);
    free(centerSumPointer);

    *iterations = totalRuns;
    return clusters;
}

double *radius(int const *k, double **points, double **centers, int const *clusters, const int *rows,
               const int *dimensions) {
    double maxDistance;
    double *radius;

    // allocate memory for radius
    radius = malloc((*k) * sizeof(double));
    if (radius == NULL) {
        printf("\nFailure to allocate room for the clusters");
        exit(0);
    }

    double pointDistance = 0;
    double sumDistance, euclideanDistance;
    for (int centerIndex = 0; centerIndex < *k; centerIndex++) {
        maxDistance = -1;
        for (int clusterIndex = 0; clusterIndex < *rows; clusterIndex++) {
            if (clusters[clusterIndex] == centerIndex) {
                sumDistance = 0;
                for (int dimension = 0; dimension < *dimensions; dimension++) {
                    // Point distance from center
                    pointDistance = points[clusterIndex][dimension] - centers[centerIndex][dimension];
                    // Power of pointDistance used to calculate Euclidean Distance
                    pointDistance = pointDistance * pointDistance;
                    // Add to sum
                    sumDistance = sumDistance + pointDistance;
                }// for dimensions
                euclideanDistance = sqrt(sumDistance);
                // Set max distance
                if (euclideanDistance > maxDistance) {
                    maxDistance = euclideanDistance;
                }
            }
        }// for clusters
        radius[centerIndex] = maxDistance;
    } // for k
    return radius;
}

char *runMethod(){
    return "omp_v2";
}