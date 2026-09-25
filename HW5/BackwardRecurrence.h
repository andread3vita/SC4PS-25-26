#ifndef BACKWARD_RECURRENCE_H
#define BACKWARD_RECURRENCE_H

#include <math.h>

static inline double backward_recurrence(int l, double x)
{
    if (l < 0)
        return NAN;
    if (l == 0)
        return 1.0;

    double p_np1 = 0.0;
    double p_n = 1.0;

    for (int n = l; n >= 1; --n) {
        double p_nm1 = ((2.0 * n + 1.0) * x * p_n
                       - (n + 1.0) * p_np1) / n;
        p_np1 = p_n;
        p_n = p_nm1;
    }

    /* Normalize the sequence so that P_0(x) = 1. */
    return 1.0 / p_n;
}

#endif
