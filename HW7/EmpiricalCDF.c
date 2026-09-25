#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 100000

int compare_double(const void *a, const void *b)
{
    double x = *(const double *)a;
    double y = *(const double *)b;

    if (x < y)
        return -1;
    else if (x > y)
        return 1;
    else
        return 0;
}

int main(int argc, char *argv[])
{
    unsigned int seed = 12345;
    double lambda = 1.5;

    double sample[N];

    for (int i = 0; i < N; i++) {

        
        double U = (double)rand_r(&seed) / ((double)RAND_MAX + 1.0);

        double Y = -log(1.0 - U) / lambda;

        sample[i] = Y;
    }

    qsort(sample, N, sizeof(double), compare_double);

    FILE *file = fopen("cdf.csv", "w");

    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    fprintf(file, "y,empirical_cdf,exact_cdf\n");

    for (int i = 0; i < N; i++) {

        double y = sample[i];

        double empirical_cdf = (double)(i + 1) / N;
        double exact_cdf = 1.0 - exp(-lambda * y);

        fprintf(file, "%.10f,%.10f,%.10f\n", y, empirical_cdf, exact_cdf);
    }

    fclose(file);

    return 0;
}