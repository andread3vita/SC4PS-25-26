#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

int main(int argc, char *argv[])
{
    if (argc != 5) {
        printf("Usage: %s a x_el y_el N\n", argv[0]);
        return 1;
    }

    float a = atof(argv[1]);
    float x_el = atof(argv[2]);
    float y_el = atof(argv[3]);
    int N = atoi(argv[4]);

    float *x = malloc(N * sizeof(float));
    float *y = malloc(N * sizeof(float));
    float *d = malloc(N * sizeof(float));

    // Since x and y have all the elements equal to x_el and y_el, respectively, this part is not necessary since d[i] = a * x_el + y_el
    for (int i = 0; i < N; i++)   
    {
        x[i] = x_el; 
        y[i] = y_el; 
    }

    for (int i = 0; i < N; i++)   d[i] = a * x[i] + y[i];
    
    bool isCorrect = true;
    for (int i = 0; i < N; i++)  
    {
       if(fabs(d[i] - (a * x_el + y_el)) > 1e-8 )
       {
            isCorrect = false;
            break;
       }

    }
    
    if (isCorrect) printf("D has all the elements equal to a * x_el + y_el\n");
    else printf("D has not all the elements equal to a * x_el + y_el\n");
    
    free(d);
    free(x);
    free(y);

    return 0;
}