#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <hdf5.h>

int main(int argc, char *argv[])
{

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
        if (d_loop[i] != d[i])
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

    hid_t fileout = H5Fcreate("results.h5", H5F_ACC_TRUNC,
                       H5P_DEFAULT, H5P_DEFAULT);

    hid_t chunks = H5Gcreate2(fileout, "chunks",
                            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    const char *names[] = {"x", "y", "d"};
    float *vectors[] = {x, y, d};

    for (size_t chunk = 0; chunk < number_of_chunks; chunk++)
    {
        size_t start = chunk * (size_t)chunk_size;
        size_t end = start + (size_t)chunk_size;

        if (end > (size_t)N)
            end = (size_t)N;

        char group_name[64];
        snprintf(group_name, sizeof(group_name), "chunk_%zu", chunk);

        hid_t group = H5Gcreate2(chunks, group_name,
                                H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

        // Save the three vector portions
        hsize_t dims[1] = {end - start};
        hid_t space = H5Screate_simple(1, dims, NULL);

        for (int v = 0; v < 3; v++)
        {
            hid_t dataset = H5Dcreate2(group, names[v], H5T_IEEE_F32LE,
                                    space, H5P_DEFAULT,
                                    H5P_DEFAULT, H5P_DEFAULT);

            H5Dwrite(dataset, H5T_NATIVE_FLOAT,
                    H5S_ALL, H5S_ALL, H5P_DEFAULT,
                    vectors[v] + start);

            H5Dclose(dataset);
        }

        H5Sclose(space);

        space = H5Screate(H5S_SCALAR);

        hid_t dataset = H5Dcreate2(group, "partial_sum", H5T_IEEE_F32LE,
                                space, H5P_DEFAULT,
                                H5P_DEFAULT, H5P_DEFAULT);

        H5Dwrite(dataset, H5T_NATIVE_FLOAT,
                H5S_ALL, H5S_ALL, H5P_DEFAULT,
                &partial_chunk_sum[chunk]);

        H5Dclose(dataset);
        H5Sclose(space);
        H5Gclose(group);
    }

    H5Gclose(chunks);

    hsize_t dims[1] = {number_of_chunks};
    hid_t space = H5Screate_simple(1, dims, NULL);

    hid_t dataset = H5Dcreate2(fileout, "partial_chunk_sum",
                            H5T_IEEE_F32LE, space,
                            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    H5Dwrite(dataset, H5T_NATIVE_FLOAT,
            H5S_ALL, H5S_ALL, H5P_DEFAULT, partial_chunk_sum);

    H5Dclose(dataset);
    H5Sclose(space);

    space = H5Screate(H5S_SCALAR);

    dataset = H5Dcreate2(fileout, "total_sum", H5T_IEEE_F32LE,
                        space, H5P_DEFAULT,
                        H5P_DEFAULT, H5P_DEFAULT);

    H5Dwrite(dataset, H5T_NATIVE_FLOAT,
            H5S_ALL, H5S_ALL, H5P_DEFAULT, &total_sum);

    H5Dclose(dataset);
    H5Sclose(space);
    H5Fclose(fileout);

    free(d);
    free(d_loop);
    free(partial_chunk_sum);
    free(x);
    free(y);

    return 0;
}