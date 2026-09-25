# Homework 7

This directory contains five independent random-number and Monte Carlo
programs. From this directory, compile them with:

```bash
gcc RandomNumbers.c -o RandomNumbers
gcc Uniform.c -lm -o Uniform
gcc PIestimator.c -lm -o PIestimator
gcc EmpiricalCDF.c -lm -o EmpiricalCDF
gcc InverseSampling.c -lm -o InverseSampling
```

Run the executables as follows:

```bash
./RandomNumbers 10
./Uniform
./PIestimator 1000000
./InverseSampling
./EmpiricalCDF
```

`RandomNumbers` and `PIestimator` take the number of trials as their argument.
The other programs use sample counts defined in their source files.
`EmpiricalCDF` writes `cdf.csv`; to regenerate its plot, install Python 3 with
`pandas` and `matplotlib`, then run:

```bash
python3 plotCDF.py
```

This creates (or replaces) `cdf.png`.
