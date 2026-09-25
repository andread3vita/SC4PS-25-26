#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "HighPrecision.h"

int main(int argc, char **argv)
{
    
    if (argc != 4) {
        fprintf(stderr, "Usage: %s theta_rad phi_rad l\n", argv[0]);
        fprintf(stderr, "       theta_rad and phi_rad are angles in radians.\n");
        return EXIT_FAILURE;
    }

    double theta = atof(argv[1]);
    double phi = atof(argv[2]);
    double l = atof(argv[3]);

    double legendre = high_precision((int) l, cos(theta));
    double normalization = sqrt((2.0 * l + 1.0) / (4.0 * acos(-1.0)));
    double y_l0 = normalization * legendre;

    printf("Y_%d0(theta = %.17g rad, phi = %.17g rad) = %.17e\n",
           (int) l, theta, phi, y_l0);
    
    return 0;
}
