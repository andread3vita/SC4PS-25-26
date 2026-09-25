#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

#define N 100000
#define BINS 25
#define MAX_Y 10.0

int main(int argc, char *argv[])
{

    unsigned int seed = 12345;
    const double bin_width = MAX_Y / BINS;
    double lambda = 1.5;

    int histogram[BINS] = {0};
    for (int i = 0; i < N; i++) {

        double U = (double)rand_r(&seed) / RAND_MAX;
        
        double Y = -log(1.0 - U) / lambda;

        /* Put Y into histogram */
        if (Y < MAX_Y) {
            
            int bin = (int)(Y / bin_width);
            histogram[bin]++;
        }
    }

    printf("%-10s %-20s %-20s\n",
           "y", "Histogram Density", "Theoretical PDF");

    printf("------------------------------------------------------\n");

    for (int i = 0; i < BINS; i++) {
        
        double hist_density = histogram[i] / (N * bin_width);

        double y = (i + 0.5) * bin_width;
        double pdf = lambda * exp(-lambda * y);

        printf("%-10.3f %-20.6f %-20.6f\n",
               y, hist_density, pdf);
    }

    return 0;
}