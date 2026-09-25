#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "BackwardRecurrence.h"
#include "ForwardRecurrence.h"
#include "HighPrecision.h"

enum { MAX_DEGREE = 50 };

static const char *OUTPUT_FILE = "legendre_comparison.csv";

static int parse_x(const char *text, double *x)
{
    char *end = NULL;
    errno = 0;
    *x = strtod(text, &end);
    return errno == 0 && end != text && *end == '\0' && isfinite(*x);
}

static double relative_error(double absolute_error, double reference)
{
    if (reference == 0.0)
        return NAN;
    return absolute_error / fabs(reference);
}

static void save_results_for_x(FILE *csv, double x)
{
    for (int l = 0; l <= MAX_DEGREE; ++l) {
        /* Use the three implementations without changing their internals. */
        double reference = high_precision(l, x);
        double forward = forward_recurrence(l, x);
        double backward = backward_recurrence(l, x);

        double forward_absolute_error = fabs(forward - reference);
        double backward_absolute_error = fabs(backward - reference);
        double forward_relative_error =
            relative_error(forward_absolute_error, reference);
        double backward_relative_error =
            relative_error(backward_absolute_error, reference);

        fprintf(csv,
                "%.17g,%d,%.17e,%.17e,%.17e,%.17e,%.17e,%.17e,%.17e\n",
                x, l, reference, forward, backward,
                forward_absolute_error, forward_relative_error,
                backward_absolute_error, backward_relative_error);
    }
}

int main(int argc, char **argv)
{
    static const double default_x_values[] = {0.0, 0.1, 0.5, 0.9, 0.99};

    /* Validate all custom x values before creating/truncating the output file. */
    double *custom_x_values = NULL;
    if (argc > 1) {
        custom_x_values = malloc((size_t) (argc - 1) * sizeof(*custom_x_values));
        if (custom_x_values == NULL) {
            fprintf(stderr, "Could not allocate memory for x values.\n");
            return EXIT_FAILURE;
        }

        for (int i = 1; i < argc; ++i) {
            if (!parse_x(argv[i], &custom_x_values[i - 1])) {
                fprintf(stderr, "Invalid finite x value: %s\n", argv[i]);
                fprintf(stderr, "Usage: %s [x ...]\n", argv[0]);
                free(custom_x_values);
                return EXIT_FAILURE;
            }
        }
    }

    FILE *csv = fopen(OUTPUT_FILE, "w");
    if (csv == NULL) {
        perror(OUTPUT_FILE);
        free(custom_x_values);
        return EXIT_FAILURE;
    }

    fprintf(csv,
            "x,l,high_precision_value,forward_value,backward_value,"
            "forward_absolute_error,forward_relative_error,"
            "backward_absolute_error,backward_relative_error\n");

    if (argc == 1) {
        size_t count = sizeof(default_x_values) / sizeof(default_x_values[0]);
        for (size_t i = 0; i < count; ++i)
            save_results_for_x(csv, default_x_values[i]);
    } else {
        for (int i = 0; i < argc - 1; ++i)
            save_results_for_x(csv, custom_x_values[i]);
    }

    if (fclose(csv) != 0) {
        perror(OUTPUT_FILE);
        free(custom_x_values);
        return EXIT_FAILURE;
    }

    free(custom_x_values);
    printf("Saved Legendre comparison data to %s\n", OUTPUT_FILE);
    return EXIT_SUCCESS;
}
