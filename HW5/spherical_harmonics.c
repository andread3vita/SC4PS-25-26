#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "HighPrecision.h"

int main(int argc, char **argv)
{
    
    if (argc != 4) {
        fprintf(stderr, "Usage: %s theta phi l\n", argv[0]);
        return EXIT_FAILURE;
    }

    double theta = strtod(argv[1]);
    double phi = strtod(argv[2]);
    double l = strtol(argv[3]);

    double legendre = high_precision((int) l, cos(theta));
    double normalization = sqrt((2.0 * l + 1.0) / (4.0 * acos(-1.0)));
    double y_l0 = normalization * legendre;

    printf("Y_%ld0(%.17g, %.17g) = %.17e\n", l, theta, phi, y_l0);
    
    return 0;
}
