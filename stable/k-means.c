#include "k-means.h"

/**
 * Calculate ner centers with average per cluster
 *
 * @param points
 * @param rows
 * @param dimensions
 * @param k
 * @param centers
 * @param clusters
 */
void newCenters(double **points, const int *rows, int const *dimensions, int const *k, double **centers,
                int const *clusters, int *clusterItems);

int *run(double **points, int const *rows, int const *dimensions, int const *k, double **centers, int *iterations, double *time) {
    // indexes of row, dimension, center(k)
    int i, j, centerIndex, totalRuns = 0;

    // one dimensional array to store the cluster of each data row and numbers of each cluster
    int *clusters, *clusterItems;

    // Distance values used to calculate Euclidean Distance
    double minDistance, sumDistance, euclideanDistance, pointDistance;

    // cluster found from Euclidean Distance
    int cluster;

    // flag to repeat k-means process as one or more cluster has change
    int clusterChange;

    // allocate memory for clusters
    clusters = calloc((*rows), sizeof(int));
    if (clusters == NULL) {
        printf("\nFailure to allocate room for the clusters");
        exit(0);
    }

    // allocate memory for clusterItems
    clusterItems = calloc((*k), sizeof(int));
    if (clusterItems == NULL) {
        printf("\nFailure to allocate room for the clusters");
        exit(0);
    }

    //randomizeCenters(points, rows, dimensions, k, centers);

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
        }

        if (clusterChange == 1) {
            newCenters(points, rows, dimensions, k, centers, clusters, clusterItems);
        }
        totalRuns ++;
    } while (clusterChange == 1);

    clock_t toc = clock();
    *time = (double)(toc - tic) / CLOCKS_PER_SEC;

    *iterations = totalRuns;
    return clusters;
}

void newCenters(double **points, const int *rows, int const *dimensions, int const *k, double **centers, int const *clusters, int *clusterItems) {
    int column, cluster;

    // Initialize all center points
    for (cluster = 0; cluster < *k; cluster++) {
        for (column = 0; column < *dimensions; column++) {
            centers[cluster][column] = 0;
        }
        clusterItems[cluster] = 0;
    }

    // Calculate sum per column and items found per cluster
    for(cluster = 0; cluster < *rows; cluster++){
        for(column = 0; column < *dimensions; column++){
            centers[clusters[cluster]][column] = centers[clusters[cluster]][column] + points[cluster][column];
        }
        clusterItems[clusters[cluster]]++;
    }

    // Average per column for new centers
    for(cluster = 0; cluster < *k; cluster++) {
        for (column = 0; column < *dimensions; column++) {
            if (clusterItems[cluster] > 0) {
                centers[cluster][column] = centers[cluster][column] / clusterItems[cluster];
            } else {
                centers[cluster][column] = 0;
            }
        }
        clusterItems[cluster] = 0;
    }
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
    return "sequential";
}

int getNumberOfThreads(){
    int threadNumbers = 1;
    return threadNumbers;
}