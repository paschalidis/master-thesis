
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int getNumber();
double r2();

int main()
{
    srand(time(NULL)); // randomize seed

    //system("ls");
    printf("hello world\n");

    double ran = r2();
    printf("Random number: %f\n", ran);

    //double max = 4.53453, min = -2.34543;
    double min = -2.34543,  max = 4.53453;

    //double range = fabs(max) + fabs(min);
    double range = max - min;

    printf("Min: %f, Max: %f\n", min, max);

    double tempRan = ran * range;

    printf("Temp Ran: %f\n", tempRan);

    double finalRan = tempRan + min;
    printf("Final Ran: %f\n", finalRan);

    //max 4.53453
    //min -2.34543


}

int getNumber(){
	return 2;
}

double r2()
{
    return (double)rand() / (double)RAND_MAX;
}