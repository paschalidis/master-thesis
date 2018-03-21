//#ifndef KMEANS_H
//#define KMEANS_H
//#include <stdio.h>

#include <math.h>

//void fillData(int **array,  int nRow, int nCols);
void initializeClusters(int *clusters, int const *rows);
void randomizeCenters(double **points, const int *rows, int const *dimensions, int const *k, double **centers);

/**
 *
 * @param points
 * @param rows
 * @param dimensions
 * @param min_array
 * @param max_array
 */
void findMinMax(double **points, int const *rows, int const *dimensions, double *min_array, double *max_array);

/**
 *
 * @param min
 * @param max
 * @return
 */
double random(double min, double max);

int * run(double **points, int const *rows, int *dimensions, int *k)
{
    int i, j;

    // one dimensional array to store the cluster of each data row
    int *clusters;

    // centers of k-means
    double *centerPointer;
    double **centers;

    // allocate memory for clusters
    clusters = malloc((*rows) * sizeof(int));
    if (clusters == NULL)
    {
        printf("\nFailure to allocate room for the clusters");
        exit(0);
    }
    initializeClusters(clusters, rows);

    // allocate the memory for the center pointers
    centerPointer = malloc(*k * *dimensions * sizeof(double));
    if (centerPointer == NULL)
    {
        printf("\nFailure to allocate room for the center pointers");
        exit(0);
    }
    // allocate room for the pointers to the rows
    centers = malloc(*k * sizeof(double *));
    if (centers == NULL)
    {
        printf("\nFailure to allocate room for pointers");
        exit(0);
    }
    // point the pointers
    for (i = 0; i < *k; i++)
    {
        centers[i] = centerPointer + (i * *dimensions);
    }
    randomizeCenters(points, rows, dimensions, k, centers);

    printf("============== Start center =============\n");
    for(i = 0; i < *k; i++)
    {
        printf("Center %d : ", i);
        for(j = 0; j < *dimensions; j++)
        {
//            printf("Min in %i column is = %f \n", j, min_array[j]);
//            printf("Max in %i column is = %f \n", j, max_array[j]);
            printf("%f, ", centers[i][j]);
            //todo store to centers
        }
        printf("\n");
    }

    printf("============== End center =============\n");
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

    for (i = 0; i < *rows; i++)
    {
        for (j = 0; j < *dimensions; j++)
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
void initializeClusters(int *clusters, int const *rows)
{
    int i;
    for(i = 0; i < *rows; i++){
        // todo: fix it to zero after update
        clusters[i] = 2;
    }
}

/**
 * Randomize Centers of K-means for first run
 */
void randomizeCenters(double **points, const int *rows, int const *dimensions, int const *k, double **centers)
{
    int i, j;
    // one dimensional array to store the min of each data column
    double *min_array;
    // one dimensional array to store the max of each data column
    double *max_array;
    // allocate memory for min and max arrays
    min_array = malloc((*dimensions) * sizeof(double));
    max_array = malloc((*dimensions) * sizeof(double));
    findMinMax(points, rows, dimensions, min_array, max_array);

    for(i = 0; i < *k; i++)
    {
        for(j = 0; j < *dimensions; j++)
        {
            centers[i][j] = random(min_array[j], max_array[j]);
        }
    }

    free(max_array);
    free(max_array);
}

void findMinMax(double **points, int const *rows, int const *dimensions, double *min_array, double *max_array) {
    int i, j;
    double min, max;

    // Run array with points per column
    for(j = 0; j < *dimensions; j++){
        //set first value as min and max
        min = points[0][j];
        max = points[0][j];
        for(i = 0; i < *rows; i++){
            if(points[i][j] > max){
                max = points[i][j];
            }

            if(points[i][j] < min){
                min = points[i][j];
            }
        }
        //store min and max to array
        min_array[j] = min;
        max_array[j] = max;
    }
}

double random(double min, double max)
{

    double randomNumber, range , tempRan, finalRan;
    //generate random number form 0.0 to 1.0
    randomNumber = (double)rand() / (double)RAND_MAX;
    //total range number from min to max eg. from -2 to 2 is 4
    //range used it to pivot form -2 to 2 -> 0 to 4 for next step
    range = fabs(min) + fabs(max);
    //illustrate randomNumber to range
    //lets say that rand() generate 0.5 number, that is the half
    //of 0.0 to 1.0, show multiple range with randomNumber we get the
    //half in range. eg 4 * 0.5 = 2
    tempRan = randomNumber * range;
    //add the min to tempRan to get the correct random in ours range
    //so in ours example we have: 2 + (-2) = 0, that is the half in -2 to 2
    finalRan = tempRan + min;
    return finalRan;
}


//#endif