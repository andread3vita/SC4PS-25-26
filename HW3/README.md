# Homework 3

`matrixMultiplication.c` multiplies two diagonal matrices, benchmarks two loop
orders, and writes the resulting matrix to a text file.

From this directory, compile it with:

```bash
gcc matrixMultiplication.c -lm -o matrixMultiplication
```

Run it with the diagonal values `a` and `b`, the matrix size `N`, and an output
file name. Use `N >= 4`, because the program checks several fixed matrix
positions.

```bash
./matrixMultiplication 2.0 3.0 4 matrix.txt
```

The timing and validation messages are printed to the terminal; the matrix is
written to `matrix.txt` in this example.
