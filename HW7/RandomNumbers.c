#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(int argc, char *argv[])
{

    // coin
    int num_tosses = atoi(argv[1]);
    int num_heads = 0;

    srand((unsigned int)time(NULL));

    for (int i = 0; i < num_tosses; i++)
    {
        int r = rand() % 2;

        if (r == 1)
        {
            num_heads++;
        }

        double percentage_heads =
            100.0 * num_heads / (i + 1);

        printf("Result: %d, percentage of heads: %.2f%%\n",
               r, percentage_heads);
    }

    return 0;
}