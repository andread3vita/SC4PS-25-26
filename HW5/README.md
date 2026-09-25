# Legendre stability and spherical harmonics

This directory compares the forward and backward recurrences for the Legendre
polynomials with the 256-bit MPFR calculation in `HighPrecision.h`.

## Legendre error table

`LegendreStability.c` evaluates the degrees \(\ell=0,\ldots,50\) at

\[
x\in\{0.1,0.5,0.9,0.99\}.
\]

For each recurrence result \(\widehat P_\ell\), it computes

\[
E_{\rm abs}=|\widehat P_\ell-P_\ell^{\rm HP}|,
\qquad
E_{\rm rel}=\frac{E_{\rm abs}}{|P_\ell^{\rm HP}|}.
\]

The program writes the whitespace-separated file `legendre_errors.txt`.  Its
columns are `x`, `l`, the forward absolute and relative errors, and the
backward absolute and relative errors.  A relative error is `nan` when the
high-precision reference is zero.

Compile and run it with:

```bash
gcc -std=c11 -O2 -Wall -Wextra -Wpedantic \
    LegendreStability.c -lmpfr -lgmp -lm -o LegendreStability
./LegendreStability
```

## Connection to spherical harmonics

For \(m=0\),

\[
Y_{\ell0}(\theta,\phi)
=\sqrt{\frac{2\ell+1}{4\pi}}\,P_\ell(\cos\theta).
\]

Thus, an absolute error in \(P_\ell\) is multiplied by the known normalization
factor, while the relative error is unchanged wherever \(P_\ell\ne0\).

`spherical_harmonics.c` evaluates this expression using `high_precision()` from
`HighPrecision.h`.  It takes `theta`, `phi`, and `l` as command-line arguments;
angles are in radians and \(0\leq\ell\leq50\).  Although `phi` is required as
an input, \(Y_{\ell0}\) is independent of it.

```bash
gcc -std=c11 -O2 -Wall -Wextra -Wpedantic \
    spherical_harmonics.c -lmpfr -lgmp -lm -o spherical_harmonics
./spherical_harmonics 1.0 0.5 4
```

If MPFR and GMP are not installed system-wide, run `install_mpfr.sh` and then
source `activate_mpfr.sh` before compiling.
