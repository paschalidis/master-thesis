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
 * @param centerRadius
 */
void newCenters(double **points, const int *rows, int const *dimensions, int const *k, double **centers,
                int const *clusters, double *centerRadius);

int *run(double **points, int const *rows, int *dimensions, int const *k, double **centers, double *centerRadius) {
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
                # pragma omp parallel for reduction(+:sumDistance)
                for (j = 0; j < *dimensions; j++) {
                    // Point distance from center
                    pointDistance = points[i][j] - centers[centerIndex][j];
                    // Power of pointDistance used to calculate Euclidean Distance
                    pointDistance = pointDistance * pointDistance;
                    // Add to sum
                    //sumDistance = sumDistance + pointDistance;
                    sumDistance += pointDistance;
                    printf("Sum of dimension for Pointer: %d %d to Center %d %d calculated on thread %d\n", i, j, centerIndex, j, omp_get_thread_num());
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

            // Set radius of cluster
            if(minDistance > centerRadius[cluster])
            {
                centerRadius[cluster] = minDistance;
            }

            if (clusters[i] != cluster) {
                clusterChange = 1;
            }
            clusters[i] = cluster;
        }

        if (clusterChange == 1) {
            newCenters(points, rows, dimensions, k, centers, clusters, centerRadius);
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
    printf("============= initializeClusters =============\n");
    # pragma omp parallel
    {
        # pragma omp for
        for (int i = 0; i < *rows; i++) {
            printf("Cluster %d initialize on thread %d\n", i, omp_get_thread_num());
            clusters[i] = 0;
        }
    }
    printf("============= initializeClusters =============\n");
}

void randomizeCenters(double **points, const int *rows, int const *dimensions, int const *k, double **centers) {

    int *randomNumbers;
    int i,j, randomNumber, found;

    // allocate memory for random numbers
    randomNumbers = malloc((*k) * sizeof(int));
    if (randomNumbers == NULL) {
        printf("\nFailure to allocate room for the random numbers");
        exit(0);
    }

    // Make difference random numbers
    for(i = 0; i < *k; i++)
    {
        found = 0;
        do{
            randomNumber = rand() % *rows;
            for(j = i; j > 0; j--){
                if (randomNumbers[j-1] == randomNumber) {
                    found = 1;
                }
            }
        } while (found == 1);
        randomNumbers[i] = randomNumber;

        // Store points to centers
        # pragma omp parallel for private(j) shared(randomNumbers, centers, points)
        for (j = 0; j < *dimensions; j++) {
            centers[i][j] = points[randomNumber][j];
            printf("New Center %d with dimension %d set it on thread %d\n", i, j, omp_get_thread_num());
        }
    }

    free(randomNumbers);
}

void newCenters(double **points, const int *rows, int const *dimensions, int const *k, double **centers,
                int const *clusters, double *centerRadius) {
    int row, column, cluster, itemsFound;

    printf("============ New Centers ===========\n");

    // Initialize all center points
    for (cluster = 0; cluster < *k; cluster++) {
        # pragma omp parallel
        {
            # pragma omp for
            for (column = 0; column < *dimensions; column++) {
                centers[cluster][column] = 0;
                printf("Center %d %d initialize on thread %d\n", cluster, column, omp_get_thread_num());
            }
        }
        centerRadius[cluster] = 0;
    }

    // calculate the average per column
    for (cluster = 0; cluster < *k; cluster++) {
        printf("============ Cluster %d ===========\n", cluster + 1);
        itemsFound = 0;
        for (row = 0; row < *rows; row++) {
            if (clusters[row] == cluster) {
                itemsFound++;
                # pragma omp parallel
                {
                    # pragma omp for
                    for (column = 0; column < *dimensions; column++) {
                        printf("%lf ", points[row][column]);
                        // Calculate the sum per column
                        centers[cluster][column] = centers[cluster][column] + points[row][column];
                    }
                }
                printf("\n");
            }
        }

        // Average per column for new center points
        # pragma omp parallel for private(column) shared(cluster, itemsFound, centers)
        for (column = 0; column < *dimensions; column++) {
            if (itemsFound > 0) {
                centers[cluster][column] = centers[cluster][column] / itemsFound;
            }
            printf("Center %d %d average calculated on thread %d\n", cluster, column, omp_get_thread_num());
        }
        printf("Items found in cluster %d : %d\n", cluster, itemsFound);
        printf("\n");
    }
    printf("============== New Centers =============\n");
    for (cluster = 0; cluster < *k; cluster++) {
        printf("Center %d : ", cluster);
        for (column = 0; column < *dimensions; column++) {
            printf("%f, ", centers[cluster][column]);
        }
        printf("\n");
    }

    printf("============ End New Centers ===========\n");
}