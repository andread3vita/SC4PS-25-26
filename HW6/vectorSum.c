#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <hdf5.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL)
    {
        printf("Cannot open input file\n");
        return 1;
    }

    float a, x_el, y_el;
    int N, chunk_size;

    if (fscanf(file, " a = %f", &a) != 1 ||
        fscanf(file, " x_el = %f", &x_el) != 1 ||
        fscanf(file, " y_el = %f", &y_el) != 1 ||
        fscanf(file, " N = %d", &N) != 1 ||
        fscanf(file, " chunk_size = %d", &chunk_size) != 1)
    {
        printf("Invalid input file\n");
        fclose(file);
        return 1;
    }

    fclose(file);

    if (N <= 0 || chunk_size <= 0)
    {
        printf("N and chunk_size must be positive\n");
        return 1;
    }

    size_t number_of_chunks = (size_t)ceil((double)N / chunk_size);

    float *x = malloc(N * sizeof(float));
    float *y = malloc(N * sizeof(float));
    float *d = malloc(N * sizeof(float));

    // Initialize x and y
    for (int i = 0; i < N; i++)
    {
        x[i] = x_el;
        y[i] = y_el;
    }

    // Process each chunk
    float *partial_chunk_sum = malloc(number_of_chunks * sizeof(float));

    for (size_t chunk_index = 0; chunk_index < number_of_chunks; chunk_index++)
    {
        size_t current_start = chunk_index * (size_t)chunk_size;
        size_t current_end = current_start + (size_t)chunk_size;

        if (current_end > (size_t)N)
        {
            current_end = (size_t)N;
        }

        float chunk_sum = 0.0f;

        for (size_t i = current_start; i < current_end; i++)
        {
            d[i] = a * x[i] + y[i];
            chunk_sum += d[i];
        }

        partial_chunk_sum[chunk_index] = chunk_sum;
    }

    float *d_loop = malloc(N * sizeof(float));

    for (int i = 0; i < N; i++)
        d_loop[i] = a * x[i] + y[i];

    bool isCorrect = true;

    for (int i = 0; i < N; i++)
    {
        if (fabs(d_loop[i] - d[i]) > 1e-8)
        {
            isCorrect = false;
            break;
        }
    }

    if (isCorrect)
        printf("D has all the elements equal to D_loop\n");
    else
        printf("D has not all the elements equal to D_loop\n");

    // Sum all partial sums
    float total_sum = 0.0f;

    for (size_t i = 0; i < number_of_chunks; i++)
    {
        total_sum += partial_chunk_sum[i];
    }

    float loop_sum = 0.0f;

    for (int i = 0; i < N; i++)
        loop_sum += d_loop[i];

    if (loop_sum == total_sum)
        printf("loop_sum is equal to total_sum\n");
    else
        printf("loop_sum is not equal to total_sum\n");

    size_t stored_elements = number_of_chunks * (size_t)chunk_size;
    float *chunk_data = calloc(stored_elements, sizeof(float));

    for (int i = 0; i < N; i++)
        chunk_data[i] = d[i];

    hid_t fileout = H5Fcreate("output.h5", H5F_ACC_TRUNC,
                              H5P_DEFAULT, H5P_DEFAULT);

    hsize_t chunks_dims[2] = {number_of_chunks, (hsize_t)chunk_size};
    hid_t chunks_space = H5Screate_simple(2, chunks_dims, NULL);
    hid_t chunks_dataset = H5Dcreate2(fileout, "chunks", H5T_IEEE_F32LE, chunks_space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    H5Dwrite(chunks_dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, chunk_data);

    H5Dclose(chunks_dataset);
    H5Sclose(chunks_space);

    hsize_t partial_sums_dims[1] = {number_of_chunks};
    hid_t partial_sums_space = H5Screate_simple(1, partial_sums_dims, NULL);
    hid_t partial_sums_dataset = H5Dcreate2(fileout, "partial_sums",
                                            H5T_IEEE_F32LE,
                                            partial_sums_space, H5P_DEFAULT,
                                            H5P_DEFAULT, H5P_DEFAULT);

    H5Dwrite(partial_sums_dataset, H5T_NATIVE_FLOAT,
             H5S_ALL, H5S_ALL, H5P_DEFAULT, partial_chunk_sum);

    H5Dclose(partial_sums_dataset);
    H5Sclose(partial_sums_space);
    H5Fclose(fileout);

    free(chunk_data);
    free(d);
    free(d_loop);
    free(partial_chunk_sum);
    free(x);
    free(y);

    return 0;
}
