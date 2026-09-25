#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

int main(int argc, char *argv[])
{
    
    double a = atof(argv[1]);
    double b = atof(argv[2]);
    int N = atoi(argv[3]);
    char* filename = argv[4];

    double *A = malloc(N * N * sizeof(double));
    double *B = malloc(N * N * sizeof(double));
    double *C = malloc(N * N * sizeof(double));

    for (int i = 0; i < N; i++)
    {
        A[i + N * i] = a;
        B[i + N * i] = b;
        C[i + N * i] = 0.;
    }

    for (int i = 0; i < N; i++)
    {   
        for (int j = 0; j < N; j++)
        {
            for(int k = 0; k < N; k++) C[i + N * j] += A[i + N * k] * B[k + N * j];
        }
        
    }


    // Check that only the diagonal elements are equal to a * b and off-diagonal are zeros

    // first check: trace
    double trace = 0.;
    for (int i = 0; i < N; i++) trace += C[i + N * i];


    // Second check: random elements in diagonal and off-diagonal
    bool isCorrect = true;
    double tol = 1e-8;
    int checkEntries[4][2] = { {N-2, N-2}, {N-2, 2}, {N-2, N-1}, {N-1, N-2}};
    for (int t = 0; t < 4; t++) {

        int i = checkEntries[t][0];
        int j = checkEntries[t][1];

        double expected;

        if (fabs(i - j) < 1e-8){
            expected = a * b;
        } else {
            expected = 0.0;
        }

        if (fabs(C[i + N * j] - expected) > tol) {
            isCorrect = false;
            break;
        }
    }
    
    if ((fabs(trace - N * a * b) < tol) && isCorrect) printf("C has diagonal elements equal to a * b\n");
    else printf("C has not diagonal elements equal to a * b\n");

    // Benchmarking
    clock_t begin_1 = clock();
    double *C_test_1 = malloc(N * N * sizeof(double));
    for (int i = 0; i < N; i++)
    {   
        for (int j = 0; j < N; j++)
        {
            for(int k = 0; k < N; k++) C_test_1[i + N * j] += A[i + N * k] * B[k + N * j];
        }
        
    }
    clock_t end_1 = clock();
    double time_spent_1 = (double)(end_1 - begin_1) / CLOCKS_PER_SEC;

    clock_t begin_2 = clock();
    double *C_test_2 = malloc(N * N * sizeof(double));
    for (int j = 0; j < N; j++)
    {   
        for (int i = 0; i < N; i++)
        {
            for(int k = 0; k < N; k++) C_test_2[i + N * j] += A[i + N * k] * B[k + N * j];
        }
        
    }
    clock_t end_2 = clock();
    double time_spent_2 = (double)(end_2 - begin_2) / CLOCKS_PER_SEC;


    printf("Time 1: Multiplication (ijk): %f [s] \n", time_spent_1);
    printf("Time 2: Multiplication (jik): %f [s] \n", time_spent_2);


    // Save C in a file
    FILE *file;
    file = fopen(filename, "w");

    fprintf(file, "C =\n");
    for (int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            fprintf(file, "%f\t", C[i + N * j]);
            
        }
        fprintf(file, "\n");
    }

    fclose(file); 


    free(C);
    free(A);
    free(B);
    free(C_test_1);
    free(C_test_2);


    return 0;
}