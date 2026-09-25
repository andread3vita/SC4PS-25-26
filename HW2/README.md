# Homework 2

`vectorSum.c` computes the element-wise operation
`d[i] = a * x[i] + y[i]` and checks the result.

From this directory, compile it with:

```bash
gcc vectorSum.c -o vectorSum -lm
```

Run it by supplying `a`, the common value of the elements of `x`, the common
value of the elements of `y`, and the vector length `N`:

```bash
./vectorSum 3 0.1 7.1 100
```
