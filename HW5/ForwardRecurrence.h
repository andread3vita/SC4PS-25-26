#ifndef FORWARD_RECURRENCE_H
#define FORWARD_RECURRENCE_H

#include <math.h>

static inline double forward_recurrence(int l, double x)
{
    if (l < 0)
        return NAN;
    if (l == 0)
        return 1.0;
    if (l == 1)
        return x;

    double p_nm2 = 1.0;
    double p_nm1 = x;

    for (int n = 2; n <= l; ++n) {
        double p_n = ((2.0 * n - 1.0) * x * p_nm1
                    - (n - 1.0) * p_nm2) / n;
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }

    return p_nm1;
}

#endif
