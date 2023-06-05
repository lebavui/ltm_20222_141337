#include <stdio.h>
#include <time.h>
static long num_steps = 100000;

int main()
{
    int i;
    double x, sum = 0.0;
    double step = 1.0 / num_steps;

    struct timespec start, stop;
    clock_gettime(CLOCK_REALTIME, &start);

    for (i = 0; i < num_steps; i++)
    {
        x = (i + 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }

    clock_gettime(CLOCK_REALTIME, &stop);
    double result = (stop.tv_sec - start.tv_sec) * 1e6 + (stop.tv_nsec - start.tv_nsec) / 1e3; // in microseconds

    printf("Elapsed time: %f us\n", result);

    double pi = sum * step;
    printf("PI = %.10f\n", pi);
    return 0;
}