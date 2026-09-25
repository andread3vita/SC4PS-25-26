#include <stdio.h>
#include <stdlib.h>
#include <mpfr.h>

double high_precision(int l, double x)
{
    mpfr_t xm, p0, p1, p2, tmp1, tmp2;

    mpfr_inits2(256, xm, p0, p1, p2, tmp1, tmp2, (mpfr_ptr) 0);

    mpfr_set_d(xm, x, MPFR_RNDN);

    /* P_0(x) = 1 */
    mpfr_set_ui(p0, 1, MPFR_RNDN);

    if (l == 0) {
        double res = mpfr_get_d(p0, MPFR_RNDN);
        mpfr_clears(xm, p0, p1, p2, tmp1, tmp2, (mpfr_ptr) 0);
        return res;
    }

    /* P_1(x) = x */
    mpfr_set(p1, xm, MPFR_RNDN);

    if (l == 1) {
        double res = mpfr_get_d(p1, MPFR_RNDN);
        mpfr_clears(xm, p0, p1, p2, tmp1, tmp2, (mpfr_ptr) 0);
        return res;
    }

    for (int n = 2; n <= l; n++) {

        /* tmp1 = (2n - 1) * x * P_(n-1) */
        mpfr_mul(tmp1, xm, p1, MPFR_RNDN);
        mpfr_mul_ui(tmp1, tmp1, 2*n - 1, MPFR_RNDN);

        /* tmp2 = (n - 1) * P_(n-2) */
        mpfr_mul_ui(tmp2, p0, n - 1, MPFR_RNDN);

        /* p2 = tmp1 - tmp2 */
        mpfr_sub(p2, tmp1, tmp2, MPFR_RNDN);

        /* p2 /= n */
        mpfr_div_ui(p2, p2, n, MPFR_RNDN);

        /* shift */
        mpfr_set(p0, p1, MPFR_RNDN);
        mpfr_set(p1, p2, MPFR_RNDN);
    }

    double res = mpfr_get_d(p2, MPFR_RNDN);

    mpfr_clears(xm, p0, p1, p2, tmp1, tmp2, (mpfr_ptr) 0);

    return res;
}