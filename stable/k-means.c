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
                int const *clusters);

int *run(double **points, int const *rows, int *dimensions, int const *k, double **centers) {
    // indexes of row, dimension, center(k)
    int i, j, centerIndex;

    // one dimensional array to store the cluster of each data row
    int *clusters;

    // Distance values used to calculate Euclidean Distance
    double minDistance, sumDistance, euclideanDistance, pointDistance;

    // cluster found from Euclidean Distance
    int cluster;

    // flag to repeat k-means process as one or more cluster has change
    int clusterChange;

    // K-means max allowed runs and counter for runs
    int maxAllowedRuns = 100;
    int currentRuns = 0;

    // allocate memory for clusters
    clusters = malloc((*rows) * sizeof(int));
    if (clusters == NULL) {
        printf("\nFailure to allocate room for the clusters");
        exit(0);
    }
    initializeClusters(clusters, rows);

    randomizeCenters(points, rows, dimensions, k, centers);

    printf("============== Initialize Centers =============\n");
    for (i = 0; i < *k; i++) {
        printf("Center %d: ", i + 1);
        for (j = 0; j < *dimensions; j++) {
            printf("%f ", centers[i][j]);
        }
        printf("\n");
    }
    printf("============== Initialize center =============\n");

    // K-means steps
    // While you have change in clusters continue
    // calculate for each point Euclidean distance from centers
    // for each row
    // for each dimension
    // for each center -- this can be seperate function
    // calculate the distance and find the min
    // set point to cluster
    // if cluster has change update flag

    printf("============== Start k-means =============\n");
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
                printf("Distance Pointer: %d to Center %d is: %f\n", i, centerIndex, euclideanDistance);
            }
            printf("min distance is %f Pointer: %d set to Cluster %d\n", minDistance, i, cluster + 1);
            if (clusters[i] != cluster) {
                clusterChange = 1;
            }
            clusters[i] = cluster;
        }

        if (clusterChange == 1) {
            newCenters(points, rows, dimensions, k, centers, clusters);
        }
        currentRuns++;
    } while (clusterChange == 1 && currentRuns < maxAllowedRuns);

    printf("============== Total Runs of k-means %d =============\n", currentRuns);

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

void initializeClusters(int *clusters, int const *rows) {
    for (int i = 0; i < *rows; i++) {
        clusters[i] = 0;
    }
}

void randomizeCenters(double **points, const int *rows, int const *dimensions, int const *k, double **centers) {
    int i, j, dimension; // Indexes
    int randomNumber; // Random Number
    int foundSameCenters, foundSameDimension; // Flag for same data

    int maxRuns = 5; // Flag set to 5 because if you cant find different point number then data have many duplicates
    int randomNumberRuns = 0;

    do{
        // Make random numbers from rows
        for(i = 0; i < *k; i++)
        {
            randomNumber = rand() % *rows;
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
}

void newCenters(double **points, const int *rows, int const *dimensions, int const *k, double **centers,
                int const *clusters) {
    int column, cluster;
    int clusterItems[*k];

    printf("============ New Centers ===========\n");

    // Initialize all center points
    for (cluster = 0; cluster < *k; cluster++) {
        for (column = 0; column < *dimensions; column++) {
            centers[cluster][column] = 0;
        }
        clusterItems[cluster] = 0;
    }

    for(cluster = 0; cluster < *rows; cluster++){
        for(column = 0; column < *dimensions; column++){
            centers[clusters[cluster]][column] = centers[clusters[cluster]][column] + points[cluster][column];
        }
        clusterItems[clusters[cluster]] = clusterItems[clusters[cluster]] + 1;
    }

    for(cluster = 0; cluster < *k; cluster++) {
        for (column = 0; column < *dimensions; column++) {
            if (clusterItems[cluster] > 0) {
                centers[cluster][column] = centers[cluster][column] / clusterItems[cluster];
            } else {
                centers[cluster][column] = 0;
            }
        }
    }

    printf("============== Centers =============\n");
    for (cluster = 0; cluster < *k; cluster++) {
        printf("Center %d : ", cluster);
        for (column = 0; column < *dimensions; column++) {
            printf("%f, ", centers[cluster][column]);
        }
        printf("\n");
    }

    printf("============ End New Centers ===========\n");
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