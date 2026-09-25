# Homework 6

`vectorSum.c` reads vector parameters from a text file, processes the vectors
in chunks, and stores the results in an HDF5 file.

The HDF5 development tools are required. Compile with the `h5cc` compiler
wrapper so that the correct include and linker flags are added automatically:

```bash
h5cc vectorSum.c -lm -o vectorSum
```

Run the program from this directory with the supplied input file:

```bash
./vectorSum input.txt
```

The program prints its validation results and creates (or replaces)
`results.h5` in the current directory. If the HDF5 command-line tools are
installed, inspect the output structure with:

```bash
h5ls -r results.h5
```
