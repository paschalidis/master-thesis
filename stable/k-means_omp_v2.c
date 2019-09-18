#include "k-means.h"

int *run(double **points, int const *rows, int const *dimensions, int const *k, double **centers, int *iterations, double *time) {
    // indexes of row, dimension, center(k)
    int totalRuns = 0;

    // one dimensional array to store the cluster of each data row
    int *clusters;

    // flag to repeat k-means process as one or more cluster has change
    int clusterChange;

    // one dimensional array to store total points per cluster
    int *clusterItems;

    // centers sums per column of k-means
    double *centerSumPointer;
    double **centersSum;

    int numberOfThreads = omp_get_max_threads();

    // Pointer for threadClusterItems
    int *threadClusterItemsPointer;
    // two dimension array to store cluster items per thread [noThread][noCluster]
    int **threadClusterItems;

    // three dimension array to store center sums per thread
    double ***threadCentersSum;

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
    if (centersSum == NULL)
    {
        printf("\nFailure to allocate room for center sum");
        exit(0);
    }
    // point the pointers for centers
    for (int i = 0; i < *k; i++)
    {
        centersSum[i] = centerSumPointer + (i * (*dimensions));
    }

    // allocate the memory for the thread cluster items pointers
    threadClusterItemsPointer = calloc(numberOfThreads * *k, sizeof(int));
    if (threadClusterItemsPointer == NULL)
    {
        printf("\nFailure to allocate room for the thread cluster items pointers");
        exit(0);
    }

    // allocate room for the pointers to the thread cluster items
    threadClusterItems = malloc(numberOfThreads * sizeof(int *));
    if (threadClusterItems == NULL)
    {
        printf("\nFailure to allocate room for thread cluster items");
        exit(0);
    }

    // point the pointers for thread cluster items
    for (int i = 0; i < numberOfThreads; i++)
    {
        threadClusterItems[i] = threadClusterItemsPointer + (i * (*k));
    }

    // allocate room for the pointers to the thread centers sum
    threadCentersSum = malloc(*dimensions * sizeof(double **));
    if (threadCentersSum == NULL)
    {
        printf("\nFailure to allocate room for the thread centers sum");
        exit(0);
    }

    for(int i = 0; i < numberOfThreads; i++)
    {
        threadCentersSum[i] = malloc(*k * sizeof(double *));
        if(threadCentersSum[i] == NULL)
        {
            printf("\nFailure to allocate room for the thread centers sum");
            exit(0);
        }

        for(int j = 0; j < *k; j++)
        {
            threadCentersSum[i][j] = calloc(*dimensions, sizeof(double));
            if(threadCentersSum[i][j] == NULL)
            {
                printf("\nFailure to allocate room for the thread centers sum");
                exit(0);
            }
        }
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
    double start_time = omp_get_wtime();
    do {
        clusterChange = 0;
        # pragma omp parallel for shared(clusters, points, centers, rows, k, dimensions, threadClusterItems, threadCentersSum) reduction(||:clusterChange) default(none) schedule(static)
        for (int i = 0; i < *rows; i++) {
            double minDistance = -1;
            int cluster = 0;
            for (int centerIndex = 0; centerIndex < *k; centerIndex++) {
                double sumDistance = 0;
                for (int j = 0; j < *dimensions; j++) {
                    // Point distance from center
                    double pointDistance = points[i][j] - centers[centerIndex][j];
                    // Power of pointDistance used to calculate Euclidean Distance
                    pointDistance = pointDistance * pointDistance;
                    // Add to sum
                    sumDistance = sumDistance + pointDistance;

                }
                double euclideanDistance = sqrt(sumDistance);
                // Set first distance as min distance
                if (minDistance == -1 || euclideanDistance < minDistance) {
                    minDistance = euclideanDistance;
                    cluster = centerIndex;
                }
            }

            clusterChange = clusterChange || (clusters[i] != cluster);
            clusters[i] = cluster;
            threadClusterItems[omp_get_thread_num()][clusters[i]] ++;

            for(int j = 0; j < *dimensions; j++){
                threadCentersSum[omp_get_thread_num()][clusters[i]][j] += points[i][j];
            }
        }

        if (clusterChange == 1) {
            // main thread reduction all cluster items and center sums per thread
            for(int i = 0; i < numberOfThreads; i++)
            {
                for(int j = 0; j < *k; j++)
                {
                    clusterItems[j] += threadClusterItems[i][j];
                    for(int z = 0; z < *dimensions; z++)
                    {
                        centersSum[j][z] += threadCentersSum[i][j][z];
                        threadCentersSum[i][j][z] = 0;
                    }
                    threadClusterItems[i][j] = 0;
                }
            }

            // Average per column for new centers
            for (int centerIndex = 0; centerIndex < *k; centerIndex++) {
                for (int j = 0; j < *dimensions; j++) {
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

    double end_time = omp_get_wtime();
    *time = end_time - start_time;

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

int getNumberOfThreads(){
    int threadNumbers = omp_get_max_threads();
    return threadNumbers;
}