#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    int num_tosses = atoi(argv[1]);


    // Two independent RNG streams
    unsigned int seed_x = 12345;
    unsigned int seed_y = 67890;

    int N_inside = 0;
    double pi = acos(-1.0);
    double final_pi_est = 0;
    double err_final = 0;
    for (int i = 0; i < num_tosses; i++)
    {

        double x = (double)rand_r(&seed_x) / RAND_MAX;
        double y = (double)rand_r(&seed_y) / RAND_MAX;

        if (x * x + y * y <= 1.0)
        {
            N_inside++;
        }

        double pi_est = 4.0 * N_inside / (i + 1);
        final_pi_est = pi_est;
        double diff_pi = fabs(pi_est - pi)/pi * 100;
        err_final = diff_pi;

        
    }

    printf("pi_est: %.6f, trials: %d, error [\%]: %.6f\n", final_pi_est, num_tosses, err_final);

    return 0;
}