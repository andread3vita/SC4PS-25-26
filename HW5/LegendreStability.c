#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "BackwardRecurrence.h"
#include "ForwardRecurrence.h"
#include "HighPrecision.h"

int main(void)
{
    const double x_values[] = {0.0, 0.1, 0.5, 0.9, 0.99};
    const int number_of_x_values =
        (int) (sizeof(x_values) / sizeof(x_values[0]));
    FILE *output = fopen("legendre_errors.txt", "w");

    fprintf(output,
            "# x l forward_absolute forward_relative "
            "backward_absolute backward_relative\n");

    for (int i = 0; i < number_of_x_values; ++i) {
        double x = x_values[i];

        for (int l = 0; l <= 50; ++l) {
            double reference = high_precision(l, x);
            double forward_error = fabs(forward_recurrence(l, x) - reference);
            double backward_error = fabs(backward_recurrence(l, x) - reference);
            double forward_relative = reference == 0.0
                                      ? NAN
                                      : forward_error / fabs(reference);
            double backward_relative = reference == 0.0
                                       ? NAN
                                       : backward_error / fabs(reference);

            fprintf(output, "%.2f %d %.17e %.17e %.17e %.17e\n",
                    x, l, forward_error, forward_relative,
                    backward_error, backward_relative);
        }
    }

    fclose(output);
    printf("Results saved in legendre_errors.txt\n");
    return EXIT_SUCCESS;
}
