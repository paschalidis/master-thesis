#include "k-means.h"

/**
 * Set cluster to default value for first run
 *
 * @param clusters
 * @param rows
 */
void initializeClusters(int *clusters, int const *rows);

/**
 * Randomize Centers of K-means for first run
 *
 * @param points
 * @param rows
 * @param dimensions
 * @param k
 * @param centers
 */
void randomizeCenters(double **points, const int *rows, int const *dimensions, int const *k, double **centers);

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
    clusters = malloc((*rows) * sizeof(int));
    if (clusters == NULL) {
        printf("\nFailure to allocate room for the clusters");
        exit(0);
    }

    clusterItems = malloc((*k) * sizeof(int));
    if (clusterItems == NULL) {
        printf("\nFailure to allocate room for the clusters items");
        exit(0);
    }

    // allocate the memory for the center pointers
    centerSumPointer = malloc((*k) * (*dimensions) * sizeof(double));
    if (centerSumPointer == NULL)
    {
        printf("\nFailure to allocate room for the center sum pointers");
        exit(0);
    }
    // allocate room for the pointers to the centers
    centersSum = malloc((*k) * sizeof(double *));
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

    for(i = 0; i <*k; i++){
        clusterItems[i] = 0;
        for (j = 0; j < *dimensions ; ++j) {
            centersSum[i][j] = 0;
        }
    }

    initializeClusters(clusters, rows);
    randomizeCenters(points, rows, dimensions, k, centers);

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

            clusterItems[cluster]++;

            for(j = 0; j < *dimensions; j++){
                centersSum[cluster][j] = centersSum[cluster][j] + points[i][j];
            }
        }

        if (clusterChange == 1) {
            // Average per column for new centers
            for(centerIndex = 0; centerIndex < *k; centerIndex++) {
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

void initializeClusters(int *clusters, int const *rows) {
    for (int i = 0; i < *rows; i++) {
        clusters[i] = 0;
    }
}

void randomizeCenters(double **points, const int *rows, int const *dimensions, int const *k, double **centers) {
    int *randomNumbers;
    int i, j, dimension; // Indexes
    int randomNumber; // Random Number
    int foundSameRandomNumbers, foundSameCenters, foundSameDimension; // Flag for same data

    int maxRuns = 5; // Flag set to 5 because if you cant find different point number then data have many duplicates
    int randomNumberRuns = 0;

    // allocate memory for random numbers
    randomNumbers = malloc((*k) * sizeof(int));
    if (randomNumbers == NULL) {
        printf("\nFailure to allocate room for the random numbers");
        exit(0);
    }

    do{

        for(i = 0; i < *k; i++) {
            // Make random numbers from rows
            do {
                foundSameRandomNumbers = 0;
                randomNumber = rand() % *rows;
                for (j = i; j > 0; j--) {
                    if (randomNumbers[j - 1] == randomNumber) {
                        foundSameRandomNumbers = 1;
                    }
                }
            } while (foundSameRandomNumbers == 1);
            randomNumbers[i] = randomNumber;

            // Store points to centers
            for (j = 0; j < *dimensions; j++) {
                centers[i][j] = points[randomNumber][j];
            }
        }

        // Check for same centers - duplicate data
        foundSameCenters = 0;
        for(i = 0; i < *k; i++){
            for(j = *k -1; j > i; j--){
                foundSameDimension = 1;
                for(dimension = 0; dimension < *dimensions; dimension++){
                    foundSameDimension = foundSameDimension && (centers[i][dimension] == centers[j][dimension]);
                }
                foundSameCenters = foundSameCenters || foundSameDimension;
            }
        }
        randomNumberRuns ++;
    } while (foundSameCenters == 1 && maxRuns > randomNumberRuns);
    free(randomNumbers);
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
    return "sequential_v2";
}