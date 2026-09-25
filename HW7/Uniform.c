#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(void)
{
    int N = 1000000;      // number of samples
    int num_bins = 20;   // number of histogram bins

    int bins[20] = {0};

    srand((unsigned int)time(NULL));

    for (int i = 0; i < N; i++)
    {
        double U = (double)rand() / RAND_MAX;
        double Y = U * U;

        int bin = (int)(Y * num_bins);
        if (bin == num_bins)
        {
            bin = num_bins - 1;
        }

        bins[bin]++;
    }

    double bin_width = 1.0 / num_bins;

    printf("y\tHistogram\tAnalytic\n");
    for (int i = 0; i < num_bins; i++)
    {
        double y = (i + 0.5) * bin_width;

        double histogram = (double)bins[i] / (N * bin_width);
        double analytic = 1.0 / (2.0 * sqrt(y));

        printf("%.4f\t%.4f\t\t%.4f\n", y, histogram, analytic);
    }

    return 0;
}