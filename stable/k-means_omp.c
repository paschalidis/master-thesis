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

int *run(double **points, int const *rows, int const *dimensions, int const *k, double **centers, int *iterations, double *time) {
    // indexes of row, dimension, center(k)
    int i, totalRuns = 0;

    // one dimensional array to store the cluster of each data row
    int *clusters;

    // flag to repeat k-means process as one or more cluster has change
    int clusterChange;

    // allocate memory for clusters
    clusters = malloc((*rows) * sizeof(int));
    if (clusters == NULL) {
        printf("\nFailure to allocate room for the clusters");
        exit(0);
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

    double start_time = omp_get_wtime();

    # pragma omp parallel shared(clusterChange, clusters, points, centers, totalRuns, rows, k, dimensions)
    {
        do {
            clusterChange = 0;
            # pragma omp parallel for private(i)
            for (i = 0; i < *rows; i++) {
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

                if (clusters[i] != cluster) {
                    clusterChange = 1;
                }
                clusters[i] = cluster;
            }

            if (clusterChange == 1) {
                newCenters(points, rows, dimensions, k, centers, clusters);
            }
            totalRuns ++;
        } while (clusterChange == 1);
    }

    double end_time = omp_get_wtime();
    *time = end_time - start_time;

    *iterations = totalRuns;
    return clusters;
}

void initializeClusters(int *clusters, int const *rows) {
    # pragma omp parallel
    {
        # pragma omp for
        for (int i = 0; i < *rows; i++) {
            clusters[i] = 0;
        }
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
}

void newCenters(double **points, const int *rows, int const *dimensions, int const *k, double **centers,
                int const *clusters) {
    int column, cluster;
    int clusterItems[*k];

    // Initialize all center points
    # pragma omp parallel for private(cluster, column) shared(centers, clusterItems)
    for (cluster = 0; cluster < *k; cluster++) {
        for (column = 0; column < *dimensions; column++) {
            centers[cluster][column] = 0;
        }
        clusterItems[cluster] = 0;
    }

    // Calculate sum per column and items found per cluster
    # pragma omp parallel for private(cluster, column) shared(centers, clusterItems, points)
    for(cluster = 0; cluster < *rows; cluster++){
        for(column = 0; column < *dimensions; column++){
            centers[clusters[cluster]][column] = centers[clusters[cluster]][column] + points[cluster][column];
        }
        clusterItems[clusters[cluster]] = clusterItems[clusters[cluster]] + 1;
    }

    // Average per column for new centers
    # pragma omp parallel for private(cluster, column) shared(centers, clusterItems)
    for(cluster = 0; cluster < *k; cluster++) {
        for (column = 0; column < *dimensions; column++) {
            if (clusterItems[cluster] > 0) {
                centers[cluster][column] = centers[cluster][column] / clusterItems[cluster];
            } else {
                centers[cluster][column] = 0;
            }
        }
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
    return "omp";
}